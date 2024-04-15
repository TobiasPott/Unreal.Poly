// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorBase.h"
#include "Selection/SelectorVisualiserBase.h"
#include "Engine/World.h"

// Sets default values
ASelectorBase::ASelectorBase()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ASelectorBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->VisualiserClass))
	{
		FTransform Transform = FTransform();
		UClass* Class = this->VisualiserClass.Get();
		UWorld* World = this->GetWorld();

		FActorSpawnParameters Params = FActorSpawnParameters();
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// spawn and init visualiser
		ASelectorVisualiserBase* Visualiser = World->SpawnActor<ASelectorVisualiserBase>(Class, Params);
		Visualiser->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
		Visualiser->Selector = this;
		Visualiser->Init();
	}

}

bool ASelectorBase::IsSelected_Implementation(USelectableBase* InSelectable)
{
	return this->Selection.Contains(InSelectable);
}

void ASelectorBase::Select_Implementation(USelectableBase* InSelectable, bool& IsSelected)
{
	if (!Selection.IsEmpty() && IsSingleSelection)
		this->ClearSelection();

	if (!IsValid(InSelectable))
	{
		IsSelected = false;
		return;
	}
	Selection.AddUnique(InSelectable);
	IsSelected = Selection.Contains(InSelectable);

	if (IsSelected)
		if (this->SelectableSelected.IsBound())
			this->SelectableSelected.Broadcast(this, InSelectable);
}

void ASelectorBase::Deselect_Implementation(USelectableBase* InSelectable, bool& IsSelected)
{
	if (!Selection.Contains(InSelectable))
	{
		IsSelected = false;
		return;
	}

	Selection.Remove(InSelectable);
	IsSelected = Selection.Contains(InSelectable);

	if (IsValid(InSelectable))
	{
		InSelectable->ChangeState(IsSelected);
	}

	if (!IsSelected)
		if (this->SelectableDeselected.IsBound())
			this->SelectableDeselected.Broadcast(this, InSelectable);

}

void ASelectorBase::Replace_Implementation(USelectableBase* InSelectable, bool& IsSelected)
{
	this->ClearSelection();
	this->Select(InSelectable, IsSelected);
}

void ASelectorBase::ClearSelection_Implementation()
{
	bool bIsSelected = false;
	for (int i = Selection.Num() - 1; i >= 0; i--)
	{
		USelectableBase* Selected = Selection[i];
		if (IsValid(Selected))
			this->Deselect(Selected, bIsSelected);
	}
	Selection.Empty(0);
}

