// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SceneActor.h"
#include "Engine/World.h"

// Sets default values
ASceneActor::ASceneActor()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetMobility(EComponentMobility::Movable);
	SetRootComponent(SceneComponent);
}

void ASceneActor::Add(AActor* InActor, bool bAttach)
{
	if (!AssociatedActors.Contains(InActor))
	{
		AssociatedActors.AddUnique(InActor);
		if (bAttach)
		{
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
			InActor->AttachToActor(this, Rules);
		}
	}
}

void ASceneActor::Remove(AActor* InActor, bool bDetach)
{
	if (AssociatedActors.Contains(InActor))
	{
		AssociatedActors.Remove(InActor);
		if (bDetach)
		{
			if (InActor->GetAttachParentActor() == this)
				InActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}
