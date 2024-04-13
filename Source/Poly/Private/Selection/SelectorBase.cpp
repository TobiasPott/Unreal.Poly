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

	// ToDo: @tpott: Add spawn of visualiser by TSubclassOf<ASelectorBase> on  begin play based of member 
	//			do not spawn visualiser if member is null/NONE

}

bool ASelectorBase::IsSelected_Implementation(AActor* InActor)
{
	return this->Selection.Contains(InActor);
}

void ASelectorBase::Select_Implementation(AActor* InActor, bool& IsSelected)
{
	if (!Selection.IsEmpty() && IsSingleSelection)
		this->ClearSelection();

	if (!IsValid(InActor))
	{
		IsSelected = false;
		return;
	}
	Selection.AddUnique(InActor);
	IsSelected = Selection.Contains(InActor);

	if (IsSelected)
		this->ActorSelected.Broadcast(this, InActor);
}

void ASelectorBase::Deselect_Implementation(AActor* InActor, bool& IsSelected)
{
	if (!Selection.Contains(InActor))
	{
		IsSelected = false;
		return;
	}

	Selection.Remove(InActor);
	IsSelected = Selection.Contains(InActor);

	if (IsValid(InActor))
	{
		UActorComponent* Component = InActor->GetComponentByClass(USelectableBase::StaticClass());
		USelectableBase* Selectable = Cast<USelectableBase>(Component);
		if (IsValid(Selectable))
			Selectable->ChangeState(IsSelected);
	}

	if (!IsSelected)
		this->ActorDeselected.Broadcast(this, InActor);

}

void ASelectorBase::Replace_Implementation(AActor* InActor, bool& IsSelected)
{
	this->ClearSelection();
	this->Select(InActor, IsSelected);
}

void ASelectorBase::ClearSelection_Implementation()
{
	bool bIsSelected = false;
	for (int i = Selection.Num() - 1; i >= 0; i--)
	{
		AActor* Actor = Selection[i];
		if (IsValid(Actor))
			this->Deselect(Actor, bIsSelected);
	}
	Selection.Empty(0);
}

