// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SceneActor.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Functions/Poly_ActorFunctions.h"

// Sets default values
ASceneActor::ASceneActor()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = UPoly_ActorFunctions::CreateDefaultSceneComponent<USceneComponent>(this, "DefaultSceneRoot", EComponentMobility::Static);

	IdentifierComponent = CreateDefaultSubobject<UIdentifierComponent>(TEXT("IdentifierComponent"));
}

void ASceneActor::BeginPlay()
{
	Super::BeginPlay();
	IdentifierComponent->SetAutoIdentifier(UKismetSystemLibrary::GetObjectName(this));
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
			InActor->OnDestroyed.AddDynamic(this, &ASceneActor::OnActorDestroyed);
		}

		// fire changed event
		this->OnSceneChanged();
	}
}

void ASceneActor::Remove(AActor* InActor, bool bDetach)
{
	if (AssociatedActors.Contains(InActor))
	{
		InActor->OnDestroyed.RemoveDynamic(this, &ASceneActor::OnActorDestroyed);
		AssociatedActors.Remove(InActor);
		if (bDetach)
		{
			if (InActor->GetAttachParentActor() == this)
				InActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		// fire changed event
		this->OnSceneChanged();
	}
}

void ASceneActor::OnActorDestroyed(AActor* DestroyedActor)
{
	if (AssociatedActors.Contains(DestroyedActor))
	{
		DestroyedActor->OnDestroyed.RemoveDynamic(this, &ASceneActor::OnActorDestroyed);
		AssociatedActors.Remove(DestroyedActor);

		// fire changed event
		this->OnSceneChanged();
	}
}
