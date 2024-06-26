// Fill out your copyright notice in the Description page of Project Settings.

#include "Gizmos/GizmoBaseActor.h"
#include "Functions/Poly_BaseFunctions.h"
#include "Functions/Poly_MeshEditFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshTransformFunctions.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "Actions/Categories/EditActorActions.h"
#include "Actions/ActionRunner.h"


// Sets default values
AGizmoBaseActor::AGizmoBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = UPoly_ActorFunctions::CreateDefaultSceneComponent<USceneComponent>(this, "DefaultSceneRoot", EComponentMobility::Movable);

}

void AGizmoBaseActor::CreateTranslateCore_Implementation(ATranslateGizmo*& OutTranslateCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->TranslateCore = this->GetWorld()->SpawnActor<ATranslateGizmo>(this->TranslateCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->TranslateCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutTranslateCore = this->TranslateCore;

	OutTranslateCore->TranslationChanged.AddDynamic(this, &AGizmoBaseActor::Translate_TranslationChanged);
}

void AGizmoBaseActor::CreateRotateCore_Implementation(ARotateGizmo*& OutRotateCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->RotateCore = this->GetWorld()->SpawnActor<ARotateGizmo>(this->RotateCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->RotateCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutRotateCore = this->RotateCore;

	OutRotateCore->RotationChanged.AddDynamic(this, &AGizmoBaseActor::Rotate_RotationChanged);
}

void AGizmoBaseActor::CreateScaleCore_Implementation(AScaleGizmo*& OutScaleCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->ScaleCore = this->GetWorld()->SpawnActor<AScaleGizmo>(this->ScaleCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->ScaleCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutScaleCore = this->ScaleCore;

	OutScaleCore->ScaleChanged.AddDynamic(this, &AGizmoBaseActor::Scale_ScaleChanged);
}

void AGizmoBaseActor::CreateSelectCore_Implementation(ASelectGizmo*& OutSelectCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->SelectCore = this->GetWorld()->SpawnActor<ASelectGizmo>(this->SelectCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->SelectCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutSelectCore = this->SelectCore;
	// setup core with class (should be called again to change mode or class)
	this->SelectCore->Setup(ESelectionRequestMode::Marquee, this->SelectClass.Get(), true, false, false);

	OutSelectCore->Finished.AddDynamic(this, &AGizmoBaseActor::Select_Finished);

}

void AGizmoBaseActor::CreateElementsCore_Implementation(AElementsGizmo*& OutElementsCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->ElementsCore = this->GetWorld()->SpawnActor<AElementsGizmo>(this->ElementsCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->ElementsCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutElementsCore = this->ElementsCore;
	// setup core with class (should be called again to change mode or class)
	this->ElementsCore->Setup(ESelectionRequestMode::Marquee, 10000.0, EGeometryScriptMeshSelectionType::Triangles, false);

	OutElementsCore->Finished.AddDynamic(this, &AGizmoBaseActor::Elements_Finished);

}

void AGizmoBaseActor::Translate_TranslationChanged_Implementation(bool bEnded, FVector DeltaTranslation)
{
	if (!bEnded)
	{
		const FTransform Transform = UPoly_BaseFunctions::Transform_TranslationOnly(DeltaTranslation);
		this->TransformCore(Transform, false, this->TranslateCore);
		this->TransformSelection(Transform, false);
	}
	else
	{
		// check if no elements are selected, perform action on actor then
		if (ElementsCore->IsEmptySelection())
		{
			const FTransform Transform = UPoly_BaseFunctions::Transform_TranslationOnly(DeltaTranslation);
			TranslateCore->SetActorRelativeLocation(FVector::ZeroVector);
			this->AddActorWorldOffset(Transform.GetLocation());

			UTransformSelectionAction* Action = NewObject<UTransformSelectionAction>(this);
			Action->SetupWith(USelectorNames::Actors, ETransformSpace::TS_World, Transform);
			Action->SetLocation(-Transform.GetLocation());
			Action->Execute();
			Action->SetLocation(Transform.GetLocation());
			AActionRunner::RunOnAny(this, Action);
		}
		else
		{
		}
		ElementsCore->UpdateSelectionVisuals();
		// update pivot transform
		this->UpdatePivot(true, true);
	}
}

void AGizmoBaseActor::Rotate_RotationChanged_Implementation(bool bEnded, FRotator DeltaRotation)
{
	if (!bEnded)
	{
		const FTransform Transform = UPoly_BaseFunctions::Transform_RotationOnly(DeltaRotation);
		this->TransformCore(Transform, false, this->RotateCore);
		this->TransformSelection(Transform, false);
	}
	else
	{
		// check if no elements are selected, perform action on actor then
		if (ElementsCore->IsEmptySelection())
		{
			RotateCore->SetActorRelativeRotation(FRotator::ZeroRotator);
			this->UpdatePivot(false, true);

			const FTransform Transform = UPoly_BaseFunctions::Transform_RotationOnly(DeltaRotation);
			UTransformSelectionAction* Action = NewObject<UTransformSelectionAction>(this);
			Action->SetupWith(USelectorNames::Actors, ETransformSpace::TS_World, Transform);
			Action->SetRotation(Transform.GetRotation().Inverse());
			Action->Execute();
			Action->SetRotation(Transform.GetRotation());
			AActionRunner::RunOnAny(this, Action);
		}
		else
		{
		}
		ElementsCore->UpdateSelectionVisuals();
		// update pivot transform
		this->UpdatePivot(true, true);
	}
}

void AGizmoBaseActor::Scale_ScaleChanged_Implementation(bool bEnded, FVector DeltaScale)
{
	if (!bEnded)
	{
		const FTransform Transform = UPoly_BaseFunctions::Transform_ScaleOnly(DeltaScale);
		this->TransformSelection(Transform, false);
	}
	else
	{
		// check if no elements are selected, perform action on actor then
		if (ElementsCore->IsEmptySelection())
		{
			const FTransform Transform = UPoly_BaseFunctions::Transform_ScaleOnly(DeltaScale);
			UTransformSelectionAction* Action = NewObject<UTransformSelectionAction>(this);
			Action->SetupWith(USelectorNames::Actors, ETransformSpace::TS_World, Transform);
			Action->SetScale3D(-Transform.GetScale3D());
			Action->Execute();
			Action->SetScale3D(Transform.GetScale3D());
			AActionRunner::RunOnAny(this, Action);
		}
		else
		{
		}
		ElementsCore->UpdateSelectionVisuals();
		// update pivot transform
		this->UpdatePivot(true, true);
	}
}

void AGizmoBaseActor::Select_Finished_Implementation(USelectionRequest* Request, bool bSuccess)
{
	this->bHasActorSelection = bSuccess;
	// update pivot transform
	this->UpdatePivot(true, true);
}


void AGizmoBaseActor::Elements_Finished_Implementation(AElementsGizmo* Core)
{
	bool bSuccess = false;
	TArray<UPolyMeshSelection*> Selections = Core->GetPolySelection();
	if (!Selections.IsEmpty())
	{
		for (auto Selection : Selections)
		{
			if (Selection->GetMeshElementsSelection().GetNumSelected() > 0)
			{
				bSuccess = true;
				break;
			}
		}
	}
	this->bHasElementSelection = bSuccess;
	// update pivot transform
	this->UpdatePivot(true, true);
}

void AGizmoBaseActor::SetupCores()
{
	ATranslateGizmo* OutTranslateCore;
	this->CreateTranslateCore(OutTranslateCore);
	ARotateGizmo* OutRotateCore;
	this->CreateRotateCore(OutRotateCore);
	AScaleGizmo* OutScaleCore;
	this->CreateScaleCore(OutScaleCore);
	ASelectGizmo* OutSelectCore;
	this->CreateSelectCore(OutSelectCore);
	AElementsGizmo* OutElementsCore;
	this->CreateElementsCore(OutElementsCore);
}

void AGizmoBaseActor::TransformSelection(FTransform DeltaTransform, bool bInLocalSpace)
{
	if (ElementsCore->IsEmptySelection())
	{
		const ETransformSpace Space = bInLocalSpace ? ETransformSpace::TS_Local : ETransformSpace::TS_World;
		TArray<UPolySelection*> ActiveSelection = this->SelectCore->GetPolySelection();
		for (int i = 0; i < ActiveSelection.Num(); i++)
		{
			UPoly_ActorFunctions::AddActorTransform(ActiveSelection[i]->GetSelectedActor(), DeltaTransform, Space);
		}
	}
	else
	{
		DeltaTransform.SetScale3D(DeltaTransform.GetScale3D() + FVector::OneVector);
		const ETransformSpace Space = bInLocalSpace ? ETransformSpace::TS_Local : ETransformSpace::TS_World;
		TArray<UPolyMeshSelection*> ActiveSelection = this->ElementsCore->GetPolySelection();
		for (int i = 0; i < ActiveSelection.Num(); i++)
		{
			UPolyMeshSelection* Selection = Cast<UPolyMeshSelection>(ActiveSelection[i]);
			UDynamicMesh* TargetMesh = Selection->GetSelectedMesh();
			FGeometryScriptMeshSelection MeshSelection = Selection->GetMeshElementsSelection();

			UPoly_MeshEditFunctions::AddMeshElementsTransform(TargetMesh, MeshSelection, DeltaTransform, Space);
		}
	}
}

void AGizmoBaseActor::TransformCore(FTransform DeltaTransform, bool bInLocalSpace, AActor* InActor)
{
	if (IsValid(InActor))
		if (bInLocalSpace)
		{
			AActor* Selected = InActor;
			Selected->AddActorLocalOffset(DeltaTransform.GetLocation());
			Selected->AddActorLocalRotation(DeltaTransform.GetRotation());
			Selected->SetActorRelativeScale3D(Selected->GetActorRelativeScale3D() + DeltaTransform.GetScale3D());
		}
		else
		{
			AActor* Selected = InActor;
			Selected->AddActorWorldOffset(DeltaTransform.GetLocation());
			Selected->AddActorWorldRotation(DeltaTransform.GetRotation());
			Selected->SetActorScale3D(Selected->GetActorScale3D() + DeltaTransform.GetScale3D());
		}
}

void AGizmoBaseActor::ResetCoreTransform(AActor* InActor)
{
	if (IsValid(InActor))
		InActor->SetActorRelativeTransform(FTransform::Identity);
}

void AGizmoBaseActor::UpdateGizmoSpace(ETransformSpace InSpace)
{
	this->Pivot.Space = InSpace;
	this->TranslateCore->UpdateGizmoSpace(InSpace);
	this->RotateCore->UpdateGizmoSpace(InSpace);
	this->ScaleCore->UpdateGizmoSpace(InSpace);
}

void AGizmoBaseActor::UpdatePivot(bool bRefreshLocation, bool bRefreshOrientation)
{
	if (bRefreshLocation)
	{
		FVector Location = GetPivotLocation();
		this->Pivot.Location = Location;
		this->SetActorLocation(Location);
	}

	if (bRefreshOrientation)
	{
		FRotator Rotation = GetPivotOrientation();
		this->Pivot.Orientation = Rotation;

		UE_LOG(LogTemp, Warning, TEXT("UpdatePivot (Orient): %s \naggr: %s;"), *Rotation.ToString(), *UEnum::GetValueAsString(PivotOrientationAggregation))
		this->SetActorRotation(Rotation);
	}
}

FVector AGizmoBaseActor::GetPivotLocation()
{
	ETransformSpace Space = this->Pivot.Space;
	switch (this->PivotLocationAggregation)
	{
	case EGizmoPivotAggregation::PA_Custom:
		return this->Pivot.Location;

	case EGizmoPivotAggregation::PA_Identity:
		return FVector::ZeroVector;

	case EGizmoPivotAggregation::PA_CenterMedian:
	{
		// ToDo: @tpott: add distinction for 'All', 'first', 'last' sources
		if (this->PivotSelectionSource == EGizmoPivotSelectionSource::PSS_Actor && this->SelectCore->IsNotEmpty())
		{
			FVector Loc = UPoly_ActorFunctions::GetLocation(this->SelectCore->GetPolySelection(), Space, this->PivotLocationAggregation);
			//UE_LOG(LogTemp, Warning, TEXT("Pivot (Actors): %s \nsource: %s; \nlocation-aggr: %s;"), *Loc.ToString(), *UEnum::GetValueAsString(PivotSource), *UEnum::GetValueAsString(PivotLocationAggregation));
			return Loc;
		}
		else if (this->PivotSelectionSource == EGizmoPivotSelectionSource::PSS_Elements && this->ElementsCore->IsNotEmpty())
		{
			FVector Loc = UPoly_ActorFunctions::GetLocation(this->ElementsCore->GetPolySelection(), Space, this->PivotLocationAggregation);
			//UE_LOG(LogTemp, Warning, TEXT("Pivot (Elements): %s \nsource: %s; \nlocation-aggr: %s;"), *Loc.ToString(), *UEnum::GetValueAsString(PivotSource), *UEnum::GetValueAsString(PivotLocationAggregation));
			return Loc;
		}
		break;
	}

	}
	// return 'identity' rotator
	return FVector::ZeroVector;
}

FRotator AGizmoBaseActor::GetPivotOrientation()
{
	ETransformSpace Space = this->Pivot.Space;
	switch (this->PivotOrientationAggregation)
	{
	case EGizmoPivotAggregation::PA_Custom:
		return this->Pivot.Orientation;

	case EGizmoPivotAggregation::PA_Identity:
		return FRotator::ZeroRotator;

	case EGizmoPivotAggregation::PA_CenterMedian:
	{		
		// ToDo: @tpott: add distinction for 'All', 'first', 'last' sources
		if (this->PivotSelectionSource == EGizmoPivotSelectionSource::PSS_Actor && this->SelectCore->IsNotEmpty())
		{
			FRotator Rot = UPoly_ActorFunctions::GetRotation(this->SelectCore->GetPolySelection(), Space, this->PivotOrientationAggregation);
			//UE_LOG(LogTemp, Warning, TEXT("Pivot (Actors): %s \nsource: %s; \nrotationn-aggr: %s;"), *Rot.ToString(), *UEnum::GetValueAsString(PivotSource), *UEnum::GetValueAsString(PivotOrientationAggregation));
			return Rot;
		}
		else if (this->PivotSelectionSource == EGizmoPivotSelectionSource::PSS_Elements && this->ElementsCore->IsNotEmpty())
		{
			FRotator Rot = UPoly_ActorFunctions::GetRotation(this->ElementsCore->GetPolySelection(), Space, this->PivotOrientationAggregation);
			//UE_LOG(LogTemp, Warning, TEXT("Pivot (Elements): %s \nsource: %s; \nrotationn-aggr: %s;"), *Rot.ToString(), *UEnum::GetValueAsString(PivotSource), *UEnum::GetValueAsString(PivotOrientationAggregation));
			return Rot;
		}
		break;
	}

	}
	// return 'identity' rotator
	return FRotator::ZeroRotator;
}

void AGizmoBaseActor::SetPivotBehaviour(const EGizmoPivotSource InSource,
	const EGizmoPivotSelectionSource InSelectionSource,
	const EGizmoPivotAggregation InLocationAggregation,
	const EGizmoPivotAggregation InOrientationAggregation)
{
	if (InSource != EGizmoPivotSource::PS_Keep)
		this->PivotSource = InSource;
	if (InSelectionSource != EGizmoPivotSelectionSource::PSS_Keep)
		this->PivotSelectionSource = InSelectionSource;
	if (InLocationAggregation != EGizmoPivotAggregation::PA_Keep)
		this->PivotLocationAggregation = InLocationAggregation;
	if (InOrientationAggregation != EGizmoPivotAggregation::PA_Keep)
		this->PivotOrientationAggregation = InOrientationAggregation;
	//this->UpdatePivot(true, true);
}
void AGizmoBaseActor::SetPivotSource(const EGizmoPivotSource InSource)
{
	SetPivotBehaviour(InSource);
}
void AGizmoBaseActor::SetPivotSelectionSource(const EGizmoPivotSelectionSource InSelectionSource)
{
	SetPivotBehaviour(EGizmoPivotSource::PS_Keep, InSelectionSource);
}
void AGizmoBaseActor::SetPivotAggregation(const EGizmoPivotAggregation InLocationAggregation, const EGizmoPivotAggregation InOrientationAggregation)
{
	SetPivotBehaviour(EGizmoPivotSource::PS_Keep, EGizmoPivotSelectionSource::PSS_Keep, InLocationAggregation, InOrientationAggregation);
}