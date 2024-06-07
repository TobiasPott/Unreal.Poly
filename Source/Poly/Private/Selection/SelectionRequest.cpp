// Fill out your copyright notice in the Description page of Project Settings.

#include "Selection/SelectionRequest.h"

void USelectionRequest::Init(const ESelectionRequestMode InMode, const FVector2D& InFirstPoint, const FVector2D& InSecondPoint, TSubclassOf<AActor> InFilterClass, bool bInIncludeNonCollider, bool bInOnlyEnclosed)
{
	this->Actors.Reset(0);
	this->Mode = InMode;
	this->FirstPoint = InFirstPoint;
	this->SecondPoint = InSecondPoint;
	this->bIncludeNonCollider = bInIncludeNonCollider;
	this->bOnlyEnclosed = bInOnlyEnclosed;
	this->FilterClass = InFilterClass.Get();
}

void USelectionRequest::OnFinished()
{
	if (Finished.IsBound())
		Finished.Broadcast(this, !Actors.IsEmpty());
}

void USelectionRequest::SetActors(TArray<AActor*> InActors)
{
	Actors.Reset(InActors.Num());
	for (int i = 0; i < InActors.Num(); i++)
		Actors.Add(InActors[i]);
}

void USelectionRequest::SetActor(AActor* InActor)
{
	Actors.Reset(1);
	Actors.Add(InActor);
}
