// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorSubsystem.h"

ASelectorSubsystemRoot::ASelectorSubsystemRoot()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneComponent);
}


ASelectorBase* USelectorSubsystem::CreateSelector(const UObject* WorldContext)
{
	AActor* NewActor = WorldContext->GetWorld()->SpawnActor(ASelectorBase::StaticClass());
	return Cast<ASelectorBase>(NewActor);
}

void USelectorSubsystem::Init(const UObject* WorldContext)
{
	if (!IsValid(SubsystemRoot))
	{
		SubsystemRoot = WorldContext->GetWorld()->SpawnActor<ASelectorSubsystemRoot>(ASelectorSubsystemRoot::StaticClass());
	}

	for (int i = 0; i < 8; i++)
	{
		const ESelectorChannel Channel = static_cast<ESelectorChannel>(1 << i);
		if (Channel == ESelectorChannel::Disabled)
			continue;

		if (!this->Selectors.Contains(Channel))
		{
			ASelectorBase* Selector = CreateSelector(WorldContext);
			Selector->AttachToActor(SubsystemRoot, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
			this->Selectors.Add(Channel, Selector);
		}
	}
}

bool USelectorSubsystem::GetSelector(const UObject* WorldContext, ESelectorChannel Channel, ASelectorBase*& OutSelector)
{
	if (Channel == ESelectorChannel::Disabled)
	{
		OutSelector = nullptr;
		return false;
	}
	if (this->Selectors.Contains(Channel))
	{
		OutSelector = this->Selectors[Channel];
		return true;
	}
	return false;
}
