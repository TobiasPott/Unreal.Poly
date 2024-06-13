// Fill out your copyright notice in the Description page of Project Settings.

#include "Gizmos/GizmoBaseActor.h"
#include "Functions/Poly_BaseFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"
//#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"


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
	OutTranslateCore->TransformEnded.AddDynamic(this, &AGizmoBaseActor::Translate_TransformEnded);
}

void AGizmoBaseActor::CreateRotateCore_Implementation(ARotateGizmo*& OutRotateCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->RotateCore = this->GetWorld()->SpawnActor<ARotateGizmo>(this->RotateCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->RotateCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutRotateCore = this->RotateCore;

	OutRotateCore->RotationChanged.AddDynamic(this, &AGizmoBaseActor::Rotate_RotationChanged);
	OutRotateCore->TransformEnded.AddDynamic(this, &AGizmoBaseActor::Rotate_TransformEnded);
}

void AGizmoBaseActor::CreateScaleCore_Implementation(AScaleGizmo*& OutScaleCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->ScaleCore = this->GetWorld()->SpawnActor<AScaleGizmo>(this->ScaleCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->ScaleCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutScaleCore = this->ScaleCore;

	OutScaleCore->ScaleChanged.AddDynamic(this, &AGizmoBaseActor::Scale_ScaleChanged);
	OutScaleCore->TransformEnded.AddDynamic(this, &AGizmoBaseActor::Scale_TransformEnded);
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
}

void AGizmoBaseActor::Translate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform)
{
	if (bEnded)
	{
		TranslateCore->SetActorRelativeLocation(FVector::ZeroVector);
		this->AddActorWorldOffset(DeltaTransform.GetLocation());
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
}

void AGizmoBaseActor::Rotate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform)
{
	if (bEnded)
	{
		RotateCore->SetActorRelativeRotation(FRotator::ZeroRotator);
		this->UpdatePivot(false, true);
	}
}

void AGizmoBaseActor::Scale_ScaleChanged_Implementation(bool bEnded, FVector DeltaScale)
{
	if (!bEnded)
	{
		const FTransform Transform = UPoly_BaseFunctions::Transform_ScaleOnly(DeltaScale);
		this->TransformSelection(Transform, false);
	}
}

void AGizmoBaseActor::Scale_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform)
{
}

void AGizmoBaseActor::Select_Finished_Implementation(USelectionRequest* Request, bool bSuccess)
{
	this->bHasActorSelection = bSuccess;
	if (bSuccess)
	{
		this->UpdatePivot(true, true);
	}
	else
	{
	}
}


void AGizmoBaseActor::Elements_Finished_Implementation(AElementsGizmo* Core)
{
	bool bSuccess = false;
	TArray<UPolyMeshSelection*> Selections = Core->GetPolySelections();
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
	if (bSuccess)
	{
		// ToDo: @tpott: Add location and rotation determination from given selection
		this->UpdatePivot(true, true);
	}
	else
	{
	}
	// update pivot transform
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
	TArray<UPolySelection*> ActiveSelection = this->SelectCore->GetPolySelection();
	if (bInLocalSpace)
	{
		for (int i = 0; i < ActiveSelection.Num(); i++)
		{
			AActor* Selected = ActiveSelection[i]->GetSelectedActor();
			Selected->AddActorLocalOffset(DeltaTransform.GetLocation());
			Selected->AddActorLocalRotation(DeltaTransform.GetRotation());
			Selected->SetActorRelativeScale3D(Selected->GetActorRelativeScale3D() + DeltaTransform.GetScale3D());
		}
	}
	else
	{
		for (int i = 0; i < ActiveSelection.Num(); i++)
		{
			AActor* Selected = ActiveSelection[i]->GetSelectedActor();
			Selected->AddActorWorldOffset(DeltaTransform.GetLocation());
			Selected->AddActorWorldRotation(DeltaTransform.GetRotation());
			Selected->SetActorScale3D(Selected->GetActorScale3D() + DeltaTransform.GetScale3D());
		}
	}
}

void AGizmoBaseActor::TransformCore(FTransform DeltaTransform, bool bInLocalSpace, AActor* InActor)
{
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
		this->Pivot.Location = GetPivotLocationFromSelection();
		this->SetActorLocation(this->Pivot.Location);
	}

	if (bRefreshOrientation)
	{
		this->Pivot.Orientation = GetPivotOrientationFromSelection();
		this->SetActorRotation(this->Pivot.Orientation);
	}
}

EGizmoPivotSelectionSource AGizmoBaseActor::GetPivotSelectionSource()
{
	return this->PivotSelectionSource;
}

FVector AGizmoBaseActor::GetPivotLocationFromSelection()
{
	ETransformSpace Space = this->Pivot.Space;
	switch (this->PivotLocationSource)
	{
	case EGizmoPivotSource::PS_Custom:
		return this->Pivot.Location;
	case EGizmoPivotSource::PS_Self:
		return UPoly_ActorFunctions::GetLocation(this, Space);
	case EGizmoPivotSource::PS_First:
		if (SelectCore->IsNotEmpty())
			return UPoly_ActorFunctions::GetLocation(SelectCore->GetFirstSelected(), Space);
		break;
	case EGizmoPivotSource::PS_Last:
		if (SelectCore->IsNotEmpty())
			return UPoly_ActorFunctions::GetLocation(SelectCore->GetLastSelected(), Space);
		break;
	case EGizmoPivotSource::PS_Center:
	{
		if (this->PivotSelectionSource == EGizmoPivotSelectionSource::PSS_Actor
			&& bHasActorSelection)
		{
			// ToDo: @tpott: Add branch for 'Elements' PivotSelectionSource to determine position from selection
			FVector SelectionCenter, SelectionExtents;
			TArray<UPolySelection*> CurSelection = this->SelectCore->GetPolySelection();
			TArray<AActor*> Actors;
			for (auto Sel : CurSelection)
			{ Actors.AddUnique(Sel->GetSelectedActor()); }

			UGameplayStatics::GetActorArrayBounds(Actors, false, SelectionCenter, SelectionExtents);
			return SelectionCenter;
		}
		else if (this->PivotSelectionSource == EGizmoPivotSelectionSource::PSS_Elements
			&& bHasElementSelection)
		{
			TArray<UPolyMeshSelection*> Selections = this->ElementsCore->GetPolySelections();
			int Count = 0;
			FVector SelectionCenter;

			// ToDo: Refine this to determine 'median' position from selection (bounds will shift location to selection 3D center
			//			Create method to get median location of mesh elements (tri & polygroups use tri-barycentric center, vertices use vertex position)
			for (auto Selection : Selections)
			{
				FVector Center;
				if (UPoly_MeshSelectionFunctions::GetSelectionCenterOfBounds(Selection->GetSelectedMesh(), Selection->GetMeshElementsSelection(), Center))
				{
					SelectionCenter = SelectionCenter + Center;
					Count++;
				}
			}

			SelectionCenter = SelectionCenter / Count;
			UE_LOG(LogTemp, Warning, TEXT("Elements: %d / %d (%s)"), Count, Selections.Num(), *SelectionCenter.ToString());
			return SelectionCenter;
		}
	}

	default:
	case EGizmoPivotSource::PS_Identity:
		break;
	}
	// return 'identity' rotator
	return FVector::ZeroVector;
}

FRotator AGizmoBaseActor::GetPivotOrientationFromSelection()
{
	switch (this->PivotOrientationSource)
	{
	case EGizmoPivotSource::PS_Custom:
		return this->Pivot.Orientation;
	case EGizmoPivotSource::PS_Self:
		return UPoly_ActorFunctions::GetRotation(this, this->Pivot.Space);
	case EGizmoPivotSource::PS_First:
		if (SelectCore->IsNotEmpty())
			return UPoly_ActorFunctions::GetRotation(SelectCore->GetFirstSelected(), this->Pivot.Space);
		break;
	case EGizmoPivotSource::PS_Last:
		if (SelectCore->IsNotEmpty())
			return UPoly_ActorFunctions::GetRotation(SelectCore->GetLastSelected(), this->Pivot.Space);
		break;

	default:
	case EGizmoPivotSource::PS_Center:
	case EGizmoPivotSource::PS_Identity:
		// ToDo: @tpott: Add branch for 'Elements' PivotSelectionSource to determine rotation/Normal from given selection
		//					Would also apply to PS_Last and First? to use orientation from selection of first or last selected actor?
		break;
	}
	// return 'identity' rotator
	return FRotator::ZeroRotator;
}
