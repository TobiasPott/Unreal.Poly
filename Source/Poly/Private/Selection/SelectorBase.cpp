// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorBase.h"
#include "Selection/SelectableBase.h"
#include "Selection/SelectorVisualiserBase.h"
#include "PolyMeshFunctions/PolyMesh_SelectionFunctions.h"
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

	// set/init visualiser type
	this->SetVisualiser(this->VisualiserClass);
}

bool ASelectorBase::IsSelected_Implementation(AActor* InSelectable)
{
	return this->Selection.Contains(InSelectable);
}

void ASelectorBase::Select_Implementation(AActor* InSelectable, bool& IsSelected)
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

	UPolyMesh_SelectionFunctions::SetMaterialForState(InSelectable, IsSelected, nullptr, 1);


	if (IsSelected)
		if (this->SelectableSelected.IsBound())
			this->SelectableSelected.Broadcast(this, InSelectable);
}

void ASelectorBase::Deselect_Implementation(AActor* InSelectable, bool& IsSelected)
{
	if (!Selection.Contains(InSelectable))
	{
		IsSelected = false;
		return;
	}

	Selection.Remove(InSelectable);
	IsSelected = Selection.Contains(InSelectable);

	UPolyMesh_SelectionFunctions::SetMaterialForState(InSelectable, IsSelected, nullptr, 1);

	if (!IsSelected)
		if (this->SelectableDeselected.IsBound())
			this->SelectableDeselected.Broadcast(this, InSelectable);

}

void ASelectorBase::Replace_Implementation(AActor* InSelectable, bool& IsSelected)
{
	this->ClearSelection();
	this->Select(InSelectable, IsSelected);
}

void ASelectorBase::SelectAll(const TArray<AActor*>& InSelectables)
{
	bool IsSelected = false;
	for (int i = 0; i < InSelectables.Num(); i++)
		this->Select(InSelectables[i], IsSelected);
}

void ASelectorBase::DeselectAll(const TArray<AActor*>& InSelectables)
{
	bool IsSelected = false;
	for (int i = 0; i < InSelectables.Num(); i++)
		this->Deselect(InSelectables[i], IsSelected);
}

void ASelectorBase::ReplaceAll(const TArray<AActor*>& InSelectables)
{
	this->ClearSelection();
	this->SelectAll(InSelectables);
}

void ASelectorBase::ClearSelection_Implementation()
{
	bool bIsSelected = false;
	for (int i = Selection.Num() - 1; i >= 0; i--)
	{
		AActor* Selected = Selection[i];
		if (IsValid(Selected))
			this->Deselect(Selected, bIsSelected);
	}
	Selection.Empty(0);
}

void ASelectorBase::SetVisualiser(TSubclassOf<ASelectorVisualiserBase> NewVisualiserClass)
{
	// ToDo: @tpott: move visualiser creation to SetVisualiser function
	//		this should handle destruction of previous visualiser and update of new one
	if (IsValid(NewVisualiserClass))
	{
		this->VisualiserClass = NewVisualiserClass;
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

