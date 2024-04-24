// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/GizmoCore.h"


// Sets default values
AGizmoCore::AGizmoCore()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void AGizmoCore::SetGizmoHidden(const bool bHiddenInGame)
{
	if (IsValid(this))
	{
		this->SetActorHiddenInGame(bHiddenInGame);
		this->SetActorEnableCollision(!bHiddenInGame);
	}
}

// Sets default values
ATransformCore::ATransformCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ATransformCore::OnTransformChanged(const bool bEndTransform, const FTransform InDelta)
{
	if (TransformChanged.IsBound())
		TransformChanged.Broadcast(bEndTransform, InDelta);
	if (TranslationChanged.IsBound())
		TranslationChanged.Broadcast(bEndTransform, InDelta.GetLocation());
	if (RotationChanged.IsBound())
		RotationChanged.Broadcast(bEndTransform, InDelta.Rotator());
	if (ScaleChanged.IsBound())
		ScaleChanged.Broadcast(bEndTransform, InDelta.GetScale3D());
}

void ATransformCore::OnTransformEnded(const FTransform InDelta)
{
	if (TransformEnded.IsBound())
		TransformEnded.Broadcast(true, InDelta);
	if (TranslationEnded.IsBound())
		TranslationEnded.Broadcast(true, InDelta.GetLocation());
	if (RotationEnded.IsBound())
		RotationEnded.Broadcast(true, InDelta.Rotator());
	if (ScaleEnded.IsBound())
		ScaleEnded.Broadcast(true, InDelta.GetScale3D());
}
