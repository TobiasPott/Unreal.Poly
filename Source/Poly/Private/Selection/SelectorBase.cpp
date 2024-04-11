// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorBase.h"

// Sets default values
ASelectorBase::ASelectorBase()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASelectorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ASelectorBase::IsSelected_Implementation(AActor* InActor)
{
	return this->Selection.Contains(InActor);
}

