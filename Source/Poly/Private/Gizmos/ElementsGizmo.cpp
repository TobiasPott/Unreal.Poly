// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/ElementsGizmo.h"
#include "Functions/Poly_UIFunctions.h"
#include "UI/PolyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"

// Sets default values
AElementsGizmo::AElementsGizmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Movable);
	SetRootComponent(DefaultSceneRoot);

	// create subcomponents and attach them to default scene root
	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DynamicMeshComponent"));
	DynamicMeshComponent->SetupAttachment(DefaultSceneRoot);
	DynamicMeshComponent->SetVisibility(false); // make visible to display selection mesh
}

// Called when the game starts or when spawned
void AElementsGizmo::BeginPlay()
{
	Super::BeginPlay();

	// create new request if none is available yet
	Request = NewObject<USelectionRequestBase>(this);


}

void AElementsGizmo::Setup(ESelectionRequestMode InMarqueeMode, bool bInIncludeOnlyEnclosed, bool bInDisableOnFinish)
{
	MarqueeMode = InMarqueeMode;
	bIncludeOnlyEnclosed = bInIncludeOnlyEnclosed;
	bDisableOnFinish = bInDisableOnFinish;
}

void AElementsGizmo::SetEnabled(const bool bInEnable)
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

void AElementsGizmo::SetGizmoHidden(const bool bHiddenInGame)
{
	Super::SetGizmoHidden(bHiddenInGame);
	this->SetEnabled(!bHiddenInGame);
}

void AElementsGizmo::SetTargets(const TArray<AActor*>& Targets)
{
	this->Selections.Reset();
	for (int i = 0; i < Targets.Num(); i++)
	{
		AActor* Target = Targets[i];
		UBaseDynamicMeshComponent* BaseDMC = Target->GetComponentByClass<UBaseDynamicMeshComponent>();
		if (IsValid(BaseDMC))
			this->Selections.Add(Target, FGeometryScriptMeshSelection());
	}
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
	Options.PolygroupMode = EGeometryScriptPrimitivePolygroupMode::PerFace;
	Options.bFlipOrientation = false;
	Options.UVMode = EGeometryScriptPrimitiveUVMode::Uniform;

	const FTransform InvTransform = this->GetActorTransform().Inverse();

	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, InvTransform, FarPlaneCorners);
	UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals(TargetMesh);

	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, InvTransform, NearPlaneCorners);

	// Top & Bottom Faces
	TArray<FVector> TempCorners;
	TempCorners = { NearPlaneCorners[1], NearPlaneCorners[0], FarPlaneCorners[0], FarPlaneCorners[1] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, InvTransform, TempCorners);

	TempCorners = { NearPlaneCorners[3], NearPlaneCorners[2], FarPlaneCorners[2], FarPlaneCorners[3] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, InvTransform, TempCorners);

	// Left & Right Faces
	TempCorners = { NearPlaneCorners[0], NearPlaneCorners[3], FarPlaneCorners[3], FarPlaneCorners[0] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, InvTransform, TempCorners);

	TempCorners = { NearPlaneCorners[2], NearPlaneCorners[1], FarPlaneCorners[1], FarPlaneCorners[2] };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, Options, InvTransform, TempCorners);
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
		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->SecondPoint = SecondPoint;

		// update mesh used or elements selection
		this->UpdateSelectionMesh(this->FirstPoint, this->SecondPoint);

		UDynamicMesh* SelectionMesh = this->DynamicMeshComponent->GetDynamicMesh();
		const FTransform InvActorTransform = this->DynamicMeshComponent->GetComponentTransform().Inverse();
		// iterate over targets (in keys of Selections)
		TArray<AActor*> Keys;
		Selections.GetKeys(Keys);

		//for (auto& KvPair : Selections)
		for (int i = 0; i < Keys.Num(); i++)
		{
			AActor* Target = Keys[i];
			if (!IsValid(Target))
				continue;
			// get dynamic mesh component
			UBaseDynamicMeshComponent* DMC = Target->GetComponentByClass<UBaseDynamicMeshComponent>();
			if (!IsValid(DMC))
				continue;

			FTransform ToTargetTransform = InvActorTransform * Target->GetActorTransform();
			UDynamicMesh* TargetMesh = DMC->GetDynamicMesh();

			FGeometryScriptMeshSelection Selection;
			UGeometryScriptLibrary_MeshSelectionFunctions::SelectMeshElementsInsideMesh(TargetMesh, SelectionMesh, Selection, ToTargetTransform, this->SelectionType);
			Selections.Emplace(Target, Selection);

			// ! ! ! !
			// DEBUG Output
			//KvPair.Value = Selection;
			int NumSelected = 0;
			EGeometryScriptMeshSelectionType SelType;
			UGeometryScriptLibrary_MeshSelectionFunctions::GetMeshSelectionInfo(Selection, SelType, NumSelected);
			UE_LOG(LogTemp, Warning, TEXT("Selected: %d (in %d)"), NumSelected, i);

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
		this->SetEnabled(false);
	this->Request = nullptr;
}

void AElementsGizmo::Clear()
{
	this->Selections.Reset();
	this->Request = nullptr;
}
