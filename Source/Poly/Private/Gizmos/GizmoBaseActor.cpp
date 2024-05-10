// Fill out your copyright notice in the Description page of Project Settings.

#include "Gizmos/GizmoBaseActor.h"
#include "Functions/Poly_BaseFunctions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Functions/Poly_ActorFunctions.h"


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
		this->SetActorRotation(this->GetRotationFromSelection());
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

void AGizmoBaseActor::Select_Finished_Implementation(UActorSelectionRequest* Request, bool bSuccess)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	SelectorSubsystem->GetSelector(this, this->SelectorName, Selector);

	if (bSuccess)
	{
		TArray<AActor*> Actors = this->SelectCore->GetSelection();
		this->Selection.Reset();
		this->Selection.Append(Actors);
		Selector->ReplaceAll(Actors);

		FVector ArrCenter;
		FVector ArrExtents;
		UGameplayStatics::GetActorArrayBounds(Actors, false, ArrCenter, ArrExtents);
		this->SetActorLocation(ArrCenter);
	}
	else
	{
		this->Selection.Reset();
		Selector->ClearSelection();
	}
}


void AGizmoBaseActor::Elements_Finished_Implementation(AElementsGizmo* Core)
{
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
	if (bInLocalSpace)
	{
		for (int i = 0; i < this->Selection.Num(); i++)
		{
			AActor* Selected = this->Selection[i];
			Selected->AddActorLocalOffset(DeltaTransform.GetLocation());
			Selected->AddActorLocalRotation(DeltaTransform.GetRotation());
			Selected->SetActorRelativeScale3D(Selected->GetActorRelativeScale3D() + DeltaTransform.GetScale3D());
		}
	}
	else
	{
		for (int i = 0; i < this->Selection.Num(); i++)
		{
			AActor* Selected = this->Selection[i];
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
	this->Space = InSpace;
	this->TranslateCore->UpdateGizmoSpace(Space);
	this->RotateCore->UpdateGizmoSpace(Space);
	this->ScaleCore->UpdateGizmoSpace(Space);
}

FRotator AGizmoBaseActor::GetRotationFromSelection()
{
	if (this->IsWorldSpace() || Selection.IsEmpty())
		return FRotator::ZeroRotator;

	AActor* FirstSelected = Selection[0];
	return FirstSelected->GetActorRotation();
}
