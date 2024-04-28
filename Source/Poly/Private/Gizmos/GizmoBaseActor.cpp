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
}

void AGizmoBaseActor::CreateRotateCore_Implementation(ARotateGizmo*& OutRotateCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->RotateCore = this->GetWorld()->SpawnActor<ARotateGizmo>(this->RotateCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->RotateCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutRotateCore = this->RotateCore;
}

void AGizmoBaseActor::CreateScaleCore_Implementation(AScaleGizmo*& OutScaleCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->ScaleCore = this->GetWorld()->SpawnActor<AScaleGizmo>(this->ScaleCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->ScaleCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutScaleCore = this->ScaleCore;
}

void AGizmoBaseActor::CreateSelectCore_Implementation(ASelectGizmo*& OutSelectCore)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	this->SelectCore = this->GetWorld()->SpawnActor<ASelectGizmo>(this->SelectCoreClass.Get(), FTransform::Identity, SpawnParams);
	this->SelectCore->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutSelectCore = this->SelectCore;
}
