// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/ElementsGizmo.h"
#include "Functions/Poly_UIFunctions.h"
#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Functions/Poly_UIFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
#include "Selection/SelectorSubsystem.h"
#include "Selection/SelectorBase.h"
#include "Actions/ActionRunner.h"
#include "UI/PolyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UDynamicMesh.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshDecompositionFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshTransformFunctions.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "GeometryScript/MeshSpatialFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "Actions/Categories/SelectionActions.h"


// ToDo: CONSIDER: @tpott: Consider using a SplineComponent to render selected edges
// ToDo: CONSIDER: @tpott: Consider a structure to store Edge selection (will require custom FEdge type to track VertexIDs and TriangleIDs

// Sets default values
AElementsGizmo::AElementsGizmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = UPoly_ActorFunctions::CreateDefaultSceneComponent<USceneComponent>(this, "DefaultSceneRoot", EComponentMobility::Movable);


	// create subcomponents and attach them to default scene root
	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DynamicMeshComponent"));
	DynamicMeshComponent->SetupAttachment(DefaultSceneRoot);
	DynamicMeshComponent->SetVisibility(false); // make visible to display selection mesh
	DynamicMeshComponent->SetComponentTickEnabled(false);

	SelectionDynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("SelectionDynamicMeshComponent"));
	//SelectionDynamicMeshComponent->SetupAttachment(DefaultSceneRoot); // do not attach to keep at origin (free of actor location)
	SelectionDynamicMeshComponent->SetComponentTickEnabled(false);
	SelectionDynamicMeshComponent->SetGenerateOverlapEvents(false);
	SelectionDynamicMeshComponent->SetCastShadow(false);
	SelectionDynamicMeshComponent->SetAffectDistanceFieldLighting(false);
	SelectionDynamicMeshComponent->SetAffectDynamicIndirectLighting(false);

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	//InstancedStaticMeshComponent->SetupAttachment(DefaultSceneRoot); // do not attach to keep at origin (free of actor location)
	InstancedStaticMeshComponent->SetComponentTickEnabled(false);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);
	InstancedStaticMeshComponent->SetCastShadow(false);
	InstancedStaticMeshComponent->SetAffectDistanceFieldLighting(false);
	InstancedStaticMeshComponent->SetAffectDynamicIndirectLighting(false);
}

// Called when the game starts or when spawned
void AElementsGizmo::BeginPlay()
{
	Super::BeginPlay();

	// create new request if none is available yet
	Request = NewObject<USelectionRequestBase>(this);
	Pool = NewObject<UDynamicMeshPool>(UDynamicMeshPool::StaticClass());

}

void AElementsGizmo::Setup(ESelectionRequestMode InMarqueeMode, const float InDistance, const EGeometryScriptMeshSelectionType InSelectionType, const bool bInDisableOnFinish)
{
	MarqueeMode = InMarqueeMode;
	Distance = InDistance;
	SelectionType = InSelectionType;
	bDisableOnFinish = bInDisableOnFinish;
}

void AElementsGizmo::SetEnableConsumeInput(const bool bInEnable)
{
	if (!bIsEnabled && bInEnable)
	{
		// bind primary input key pressed/released events
		this->EnableInput(UGameplayStatics::GetPlayerController(this, PlayerIndex));

		FInputActionBinding& IKA_Pressed = InputComponent->BindAction(InputAction, EInputEvent::IE_Pressed, this, &AElementsGizmo::OnInputKey_Pressed);
		IKA_Pressed.bConsumeInput = true;
		IKA_Pressed.bExecuteWhenPaused = true;
		FInputActionBinding& IKA_Released = InputComponent->BindAction(InputAction, EInputEvent::IE_Released, this, &AElementsGizmo::OnInputKey_Released);
		IKA_Released.bConsumeInput = true;
		IKA_Released.bExecuteWhenPaused = true;

		// bind mouse axes events (to conume their input from other receivers)
		FInputVectorAxisBinding& Mouse2D_Axis = InputComponent->BindVectorAxis("Mouse2D");
		Mouse2D_Axis.AxisDelegate.BindDelegate(this, &AElementsGizmo::OnMouse2D);
		Mouse2D_Axis.bConsumeInput = false;
		Mouse2D_Axis.bExecuteWhenPaused = true;

		FInputAxisKeyBinding& MouseX_Axis = InputComponent->BindAxisKey("MouseX");
		MouseX_Axis.AxisDelegate.BindDelegate(this, &AElementsGizmo::OnMouseX);
		MouseX_Axis.bConsumeInput = false;
		MouseX_Axis.bExecuteWhenPaused = true;
		FInputAxisKeyBinding& MouseY_Axis = InputComponent->BindAxisKey("MouseY");
		MouseY_Axis.AxisDelegate.BindDelegate(this, &AElementsGizmo::OnMouseY);
		MouseY_Axis.bConsumeInput = false;
		MouseY_Axis.bExecuteWhenPaused = true;

	}
	else if (bIsEnabled && !bInEnable)
	{
		InputComponent->RemoveAxisBinding("Mouse2D");
		InputComponent->RemoveAxisBinding("MouseX");
		InputComponent->RemoveAxisBinding("MouseY");

		InputComponent->RemoveActionBinding(InputAction, EInputEvent::IE_Pressed);
		InputComponent->RemoveActionBinding(InputAction, EInputEvent::IE_Released);

		// bind primary input key pressed/released events
		this->DisableInput(UGameplayStatics::GetPlayerController(this, PlayerIndex));

	}

	bIsEnabled = bInEnable;
}

void AElementsGizmo::SetTargets(const TArray<AActor*>& Targets)
{
	// remove poly selections for actors no longer selected
	for (int i = this->PolySelections.Num() - 1; i >= 0; i--)
	{
		AActor* Target = this->PolySelections[i]->GetSelectedActor();
		if (!this->PolySelections.ContainsByPredicate([Target](UPolyMeshSelection* Selection) { return Selection->IsSelectedActor(Target); }))
			this->PolySelections.RemoveAt(i);
	}
	// add new poly selection instances for selected actors (reuse existing ones)
	for (int i = 0; i < Targets.Num(); i++)
	{
		AActor* Target = Targets[i];
		UBaseDynamicMeshComponent* BaseDMC = Target->GetComponentByClass<UBaseDynamicMeshComponent>();
		if (IsValid(BaseDMC))
			UPolySelection::AddByActorT<UPolyMeshSelection>(this->PolySelections, Target);
	}

	// Reset selection mesh
	this->SelectionDynamicMeshComponent->GetDynamicMesh()->Reset();
	this->InstancedStaticMeshComponent->ClearInstances();

	// Add poly mesh selection to selector 'Elements' via action
	//UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished(). SetSelection"));
	USetSelectionAction* SetSelectionAction = NewObject<USetSelectionAction>(this);
	SetSelectionAction->SetupWith(USelectorNames::Elements, this->PolySelections);
	AActionRunner::RunOnAny(this, SetSelectionAction);
}

void AElementsGizmo::ClearTargets()
{
	this->PolySelections.Empty();
	// Reset selection mesh
	this->SelectionDynamicMeshComponent->GetDynamicMesh()->Reset();
	this->InstancedStaticMeshComponent->ClearInstances();

	// clear selector 'Elements' via action
	//UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished(). ClearSelection"));
	UClearSelectionAction* ClearSelectionAction = NewObject<UClearSelectionAction>(this);
	ClearSelectionAction->SetupWith(USelectorNames::Elements);
	AActionRunner::RunOnAny(this, ClearSelectionAction);
}

void AElementsGizmo::ClearSelections()
{
	for(auto Selection: this->PolySelections)
	{
		Selection->Selection.ClearSelection();
	}
	// Reset selection mesh
	this->SelectionDynamicMeshComponent->GetDynamicMesh()->Reset();
	this->InstancedStaticMeshComponent->ClearInstances();

	// Add poly mesh selection to selector 'Elements' via action
	//UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished(). SetSelection"));
	USetSelectionAction* SetSelectionAction = NewObject<USetSelectionAction>(this);
	SetSelectionAction->SetupWith(USelectorNames::Elements, this->PolySelections);
	AActionRunner::RunOnAny(this, SetSelectionAction);
}

void AElementsGizmo::SetSelectionType(EGeometryScriptMeshSelectionType InSelectionType)
{
	this->SelectionType = InSelectionType;
	// set visualiser components hidden based on current selection type
	this->SelectionDynamicMeshComponent->SetHiddenInGame(this->SelectionType == EGeometryScriptMeshSelectionType::Vertices);
	this->InstancedStaticMeshComponent->SetHiddenInGame(this->SelectionType != EGeometryScriptMeshSelectionType::Vertices);
}
void AElementsGizmo::SetSelectionMode(EPolySelectionMode InSelectionMode)
{
	this->SelectionMode = InSelectionMode;
}

void AElementsGizmo::UpdateSelectionMesh(const FVector2D FirstScreenPoint, const FVector2D SecondScreenPoint)
{
	// get screen space origin and size
	FVector2D Origin;
	FVector2D Size;
	UPoly_UIFunctions::GetRectOriginAndSize(FirstScreenPoint, SecondScreenPoint, Origin, Size);
	// get rect corners in screen space
	FVector2D BL, BR, TR, TL;
	UPoly_UIFunctions::GetRectCorners(Origin, Size, BL, BR, TR, TL);
	// get near and far plane world corners
	TArray<FVector> NearPlaneCorners;
	TArray<FVector> FarPlaneCorners;
	UPoly_UIFunctions::GetScreenRectWorldCorners(this, this->PlayerIndex, BL, BR, TR, TL, NearPlaneCorners, FarPlaneCorners, Distance);

	// get selection mesh instance
	UDynamicMesh* TargetMesh = this->DynamicMeshComponent->GetDynamicMesh();
	TargetMesh->Reset();
	// append primitive options
	FGeometryScriptPrimitiveOptions Options;
	Options.PolygroupMode = EGeometryScriptPrimitivePolygroupMode::SingleGroup;
	Options.bFlipOrientation = false;
	Options.UVMode = EGeometryScriptPrimitiveUVMode::Uniform;

	this->DynamicMeshComponent->SetWorldTransform(FTransform::Identity);
	const FTransform AppendTransform = FTransform::Identity;

	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, AppendTransform, FarPlaneCorners);
	UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals(TargetMesh);

	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, AppendTransform, NearPlaneCorners);

	// Top & Bottom Faces
	TArray<FVector> TempCorners;
	TempCorners = { NearPlaneCorners[1], NearPlaneCorners[0], FarPlaneCorners[0], FarPlaneCorners[1] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, AppendTransform, TempCorners);

	TempCorners = { NearPlaneCorners[3], NearPlaneCorners[2], FarPlaneCorners[2], FarPlaneCorners[3] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, AppendTransform, TempCorners);

	// Left & Right Faces
	TempCorners = { NearPlaneCorners[0], NearPlaneCorners[3], FarPlaneCorners[3], FarPlaneCorners[0] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, AppendTransform, TempCorners);

	TempCorners = { NearPlaneCorners[2], NearPlaneCorners[1], FarPlaneCorners[1], FarPlaneCorners[2] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, AppendTransform, TempCorners);
}

void AElementsGizmo::UpdateSelection()
{
	const FGeometryScriptSpatialQueryOptions QueryOptions = { 0, false, this->WindingThreshold };

	UDynamicMesh* SelectByMesh = this->DynamicMeshComponent->GetDynamicMesh();

	FGeometryScriptDynamicMeshBVH TargetBVH;
	FVector RayOrigin, RayDir;
	UPoly_UIFunctions::GetScreenRay(this, this->PlayerIndex, this->SecondPoint, RayOrigin, RayDir);

	for (int i = 0; i < PolySelections.Num(); i++)
	{
		AActor* Target = PolySelections[i]->GetSelectedActor();
		if (!IsValid(Target))
			continue;
		// get dynamic mesh component
		UBaseDynamicMeshComponent* DMC = Target->GetComponentByClass<UBaseDynamicMeshComponent>();
		if (!IsValid(DMC))
			continue;

		const FTransform TargetTransform = Target->GetActorTransform();
		const FQuat TargetRotation = TargetTransform.GetRotation();
		const FTransform InvTargetTransform = TargetTransform.Inverse();
		UDynamicMesh* TargetMesh = DMC->GetDynamicMesh();
		FGeometryScriptMeshSelection Selection;

		if (this->SelectionType == EGeometryScriptMeshSelectionType::Triangles)
		{
			// select triangles
			UGeometryScriptLibrary_MeshSelectionFunctions::SelectMeshElementsInsideMesh(TargetMesh, SelectByMesh, Selection, InvTargetTransform,
				this->SelectionType, this->bInvert, 0.0, this->WindingThreshold, this->MinTrianglePoints);
			// ! ! ! !
			// DEBUG Output
			//UPoly_SelectionFunctions::LogSelectionInfo("Triangles: ", Selection);
		}
		else if (this->SelectionType == EGeometryScriptMeshSelectionType::Polygroups)
		{
			// select polygroups and convert to triangles
			FGeometryScriptMeshSelection PolyGroupSelection;
			UGeometryScriptLibrary_MeshSelectionFunctions::SelectMeshElementsInsideMesh(TargetMesh, SelectByMesh, PolyGroupSelection, InvTargetTransform,
				this->SelectionType, this->bInvert, 0.0, this->WindingThreshold, this->MinTrianglePoints);
			UGeometryScriptLibrary_MeshSelectionFunctions::ConvertMeshSelection(TargetMesh, PolyGroupSelection, Selection, EGeometryScriptMeshSelectionType::Triangles, true);
			// ! ! ! !
			// DEBUG Output
			//UPoly_SelectionFunctions::LogSelectionInfo("PolyGroups: ", PolyGroupSelection);
		}
		else
		{
			// select vertices
			UGeometryScriptLibrary_MeshSelectionFunctions::SelectMeshElementsInsideMesh(TargetMesh, SelectByMesh, Selection, InvTargetTransform,
				this->SelectionType, this->bInvert, 0.0, this->WindingThreshold, this->MinTrianglePoints);
			// ! ! ! !
			// DEBUG Output
			//UPoly_SelectionFunctions::LogSelectionInfo("Vertices: ", Selection);
		}

		if (this->SelectionType != EGeometryScriptMeshSelectionType::Vertices)
		{
			// transform rays to target's 'world' space
			const FVector TargetRayOrigin = TargetTransform.InverseTransformPosition(RayOrigin);
			const FVector TargetRayDir = TargetTransform.InverseTransformVector(RayDir);

			FGeometryScriptRayHitResult QueryResult;
			EGeometryScriptSearchOutcomePins QueryOutcome;
			UGeometryScriptLibrary_MeshSpatial::BuildBVHForMesh(TargetMesh, TargetBVH);
			UGeometryScriptLibrary_MeshSpatial::FindNearestRayIntersectionWithMesh(TargetMesh, TargetBVH, TargetRayOrigin, TargetRayDir, QueryOptions, QueryResult, QueryOutcome);

			if (QueryOutcome == EGeometryScriptSearchOutcomePins::Found)
			{
				FGeometryScriptIndexList IndexList = FGeometryScriptIndexList();
				IndexList.Reset(EGeometryScriptIndexType::Triangle);
				IndexList.List->Add(QueryResult.HitTriangleID);

				FGeometryScriptMeshSelection ClickSelection;
				UGeometryScriptLibrary_MeshSelectionFunctions::ConvertIndexListToMeshSelection(TargetMesh, IndexList, EGeometryScriptMeshSelectionType::Triangles, ClickSelection);
				if (this->SelectionType == EGeometryScriptMeshSelectionType::Polygroups)
					UGeometryScriptLibrary_MeshSelectionFunctions::ExpandMeshSelectionToConnected(TargetMesh, ClickSelection, ClickSelection, EGeometryScriptTopologyConnectionType::Polygroup);
				// add click to existig selection
				Selection.CombineSelectionInPlace(ClickSelection, EGeometryScriptCombineSelectionMode::Add);

				// DEBUG Output
				//UPoly_SelectionFunctions::LogSelectionInfo("Click: ", ClickSelection);
			}

		}

		UPolyMeshSelection* PolySelection = *this->PolySelections.FindByPredicate([Target](UPolyMeshSelection* Item) { return Item->IsSelectedActor(Target); });
		if (IsValid(PolySelection))
		{
			// perform selection combine for select and deselct modes
			switch (this->SelectionMode)
			{
			case EPolySelectionMode::Select:
			{
				FGeometryScriptMeshSelection OldSelection = PolySelection->Selection;
				OldSelection.CombineSelectionInPlace(Selection, EGeometryScriptCombineSelectionMode::Add);
				Selection = OldSelection;
				break;
			}
			case EPolySelectionMode::Deselect:
			{
				FGeometryScriptMeshSelection OldSelection = PolySelection->Selection;
				OldSelection.CombineSelectionInPlace(Selection, EGeometryScriptCombineSelectionMode::Subtract);
				Selection = OldSelection;
				break;
			}
			default:
			case EPolySelectionMode::Replace:
				break;
			}

			// get poly selection instance for target and assign current geo script selection to it
			PolySelection->LocalToWorld = Target->GetTransform();
			PolySelection->Selection = Selection;
		}

	}


}

void AElementsGizmo::UpdateSelectionVisuals()
{
	const FGeometryScriptAppendMeshOptions AppendOptions = { EGeometryScriptCombineAttributesMode::EnableAllMatching };

	// reset selection mesh
	UDynamicMesh* SelectionMesh = this->SelectionDynamicMeshComponent->GetDynamicMesh();
	SelectionMesh->Reset();
	this->SelectionDynamicMeshComponent->SetWorldTransform(FTransform::Identity);

	// check selection type (only triangles and poly groups use mesh to display)
	UDynamicMesh* TempMesh = Pool->RequestMesh();

	const FVector VertexScale = FVector(0.05, 0.05, 0.05);
	TArray<FTransform> InstancesArray;
	for (auto PolySelection : PolySelections)
	{
		AActor* Target = PolySelection->GetSelectedActor();
		UDynamicMesh* TargetMesh = PolySelection->GetSelectedMesh();
		FGeometryScriptMeshSelection TargetSelection = PolySelection->GetMeshElementsSelection();
		const FTransform TargetTransform = Target->GetActorTransform();
		const FQuat TargetRotation = TargetTransform.GetRotation();

		// ToDo: @tpott: Move this to own function to run afterwards when all selections are queried/build (use the poly selections for this!)
		EGeometryScriptMeshSelectionType TypeOfSelection = TargetSelection.GetSelectionType();
		if (TargetSelection.GetNumSelected() > 0)
		{
			if (TypeOfSelection != EGeometryScriptMeshSelectionType::Vertices)
			{
				// get selection of all before append
				UGeometryScriptLibrary_MeshDecompositionFunctions::CopyMeshSelectionToMesh(TargetMesh, TempMesh, TargetSelection, TempMesh, false);
				UGeometryScriptLibrary_MeshBasicEditFunctions::AppendMesh(SelectionMesh, TempMesh, TargetTransform, false, AppendOptions);
			}
			else if (TypeOfSelection == EGeometryScriptMeshSelectionType::Vertices)
			{
				// get vertices for selection and add transform to instannces array
				TArray<int32> IndexArray;
				EGeometryScriptMeshSelectionType OutType;
				UGeometryScriptLibrary_MeshSelectionFunctions::ConvertMeshSelectionToIndexArray(TargetMesh, TargetSelection, IndexArray, OutType);

				for (int vI = 0; vI < IndexArray.Num(); vI++)
				{
					bool bIsValid = false;
					const FVector Location = TargetTransform.TransformPosition(UGeometryScriptLibrary_MeshQueryFunctions::GetVertexPosition(TargetMesh, IndexArray[vI], bIsValid));
					InstancesArray.Add(FTransform(TargetRotation, Location, VertexScale));
				}
			}
		}
	}

	// Add vertices to instanced static mesh component
	InstancedStaticMeshComponent->ClearInstances();
	if (!InstancesArray.IsEmpty())
		InstancedStaticMeshComponent->AddInstances(InstancesArray, false, true, false);


	// return temp mesh
	Pool->ReturnMesh(TempMesh);
}


void AElementsGizmo::OnInputKey_Pressed(FKey InKey)
{
	UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, FirstPoint);
	SecondPoint = FirstPoint;

	Request = NewObject<USelectionRequestBase>(this);
	Request->Mode = MarqueeMode;
	Request->FirstPoint = FirstPoint;
	Request->SecondPoint = FirstPoint;

	APolyHUD* Hud = Cast<APolyHUD>(UGameplayStatics::GetPlayerController(this, PlayerIndex)->GetHUD());
	if (IsValid(Hud))
	{
		Hud->QueueBaseRequest(Request);
	}
	bIsMousePressed = true;
}

void AElementsGizmo::OnInputKey_Released(FKey InKey)
{
	if (bIsMousePressed && IsValid(Request))
	{
		bIsMousePressed = false;
		const APlayerController* PC = UGameplayStatics::GetPlayerController(this, this->PlayerIndex);
		const bool bShift = PC->IsInputKeyDown(EKeys::LeftShift);
		const bool bCtrl = PC->IsInputKeyDown(EKeys::LeftControl);
		if (bCtrl) this->SetSelectionMode(EPolySelectionMode::Deselect);
		else if (bShift) this->SetSelectionMode(EPolySelectionMode::Select);
		else this->SetSelectionMode(EPolySelectionMode::Replace);

		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->UpdateSecondPoint(SecondPoint);

		// update mesh used or elements selection
		this->UpdateSelectionMesh(this->FirstPoint, this->SecondPoint);
		// update actual selection from targets and selection mesh
		this->UpdateSelection();
		this->UpdateSelectionVisuals();


		bool bIsEmpty = this->PolySelections.IsEmpty();
		if (!bIsEmpty)
		{
			//UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished(). SetSelection"));
			USetElementsSelectionAction* SetSelectionAction = NewObject<USetElementsSelectionAction>(this);
			SetSelectionAction->SetupWith(USelectorNames::Elements, this->PolySelections);
			AActionRunner::RunOnAny(this, SetSelectionAction);
		}

		Request->Submit();
		this->OnFinished();
	}
}

void AElementsGizmo::OnMouseX(float AxisValue)
{
}

void AElementsGizmo::OnMouseY(float AxisValue)
{
}

void AElementsGizmo::OnMouse2D(FVector AxisValue)
{
	if (bIsMousePressed)
	{
		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->SecondPoint = SecondPoint;
	}
}

void AElementsGizmo::OnFinished()
{
	if (this->Finished.IsBound())
		this->Finished.Broadcast(this);

	if (bDisableOnFinish)
		this->SetEnableConsumeInput(false);
	this->Request = nullptr;
}

void AElementsGizmo::Clear()
{
	this->PolySelections.Reset();
	this->Request = nullptr;
}
