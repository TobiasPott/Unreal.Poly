// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectableBase.h"

// Sets default values for this component's properties
USelectableBase::USelectableBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USelectableBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USelectableBase::OnActorClicked_Implementation(AActor* TouchedActor, FKey ButtonPressed)
{
}

void USelectableBase::ChangeState_Implementation(const bool bIsSelected)
{
}

