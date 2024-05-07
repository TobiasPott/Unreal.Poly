// Fill out your copyright notice in the Description page of Project Settings.

#include "Gizmos/GizmoBaseActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGizmoBaseActor::AGizmoBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

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

	OutSelectCore->Finished.AddDynamic(this, &AGizmoBaseActor::Select_Finished);

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
