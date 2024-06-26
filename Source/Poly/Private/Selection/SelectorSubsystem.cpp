// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorSubsystem.h"
#include "Functions/Poly_ActorFunctions.h"

ASelectorSubsystemRoot::ASelectorSubsystemRoot()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = UPoly_ActorFunctions::CreateDefaultSceneComponent<USceneComponent>(this, "DefaultSceneRoot", EComponentMobility::Static);

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

	TArray<FName> SelectorNames = { USelectorNames::Default, USelectorNames::Second, USelectorNames::Third, USelectorNames::Fourth,
									USelectorNames::Actors, USelectorNames::Elements };
	for (int i = 0; i < SelectorNames.Num(); i++)
	{
		// Add selector (using index as stencil (starting with 1 (i+1))
		ASelectorBase* Selector = AddSelector(SelectorNames[i], static_cast<uint8>(i + 1));
		// set default (outline post process) viualiser for all selectors except 'Elements'
		if (i != 5)
			Selector->SetVisualiser(ASelectorVisualiserBase::StaticClass());
	}

}

bool USelectorSubsystem::HasSelector(FName Name)
{
	return this->Selectors.Contains(Name);
}

bool USelectorSubsystem::IsEmpty(FName Name)
{
	ASelectorBase* Selector;
	if (GetSelector(this, Name, Selector))
	{
		return Selector->IsEmpty();
	}
	// no selector means emtpy selection
	return true;
}

ASelectorBase* USelectorSubsystem::AddSelector(FName Name, uint8 Stencil)
{
	if (!this->Selectors.Contains(Name))
	{
		ASelectorBase* Selector = CreateSelector(SubsystemRoot);
		Selector->AttachToActor(SubsystemRoot, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
		Selector->Stencil = static_cast<uint8>(Stencil);
		this->Selectors.Add(Name, Selector);
		return Selector;
	}
	return this->Selectors[Name];
}

bool USelectorSubsystem::GetSelector(const UObject* WorldContext, FName Name, ASelectorBase*& OutSelector)
{
	if (Name.IsNone())
	{
		OutSelector = nullptr;
		return false;
	}
	if (this->Selectors.Contains(Name))
	{
		OutSelector = this->Selectors[Name];
		return true;
	}
	return false;
}
