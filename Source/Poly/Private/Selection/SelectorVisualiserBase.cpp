// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorVisualiserBase.h"
#include "Selection/SelectorBase.h"
#include "PolyMeshFunctions/PolyMesh_SelectionFunctions.h"

// Sets default values
ASelectorVisualiserBase::ASelectorVisualiserBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASelectorVisualiserBase::BeginPlay()
{
	Super::BeginPlay();

	this->Init();
}

void ASelectorVisualiserBase::Init()
{
	if (!bIsInitialised && IsValid(Selector))
	{
		Selector->ActorSelected.AddDynamic(this, &ASelectorVisualiserBase::OnActorSelected);
		Selector->ActorDeselected.AddDynamic(this, &ASelectorVisualiserBase::OnActorDeselected);
		Selector->OnDestroyed.AddDynamic(this, &ASelectorVisualiserBase::OnSelectorDestroyed);
		bIsInitialised = true;
	}
}

void ASelectorVisualiserBase::OnActorSelected_Implementation(ASelectorBase* InSelector, AActor* InActor)
{
	UPolyMesh_SelectionFunctions::SetMaterialForState(InActor, true, this->OverlayMaterial, this->StencilValue);
}

void ASelectorVisualiserBase::OnActorDeselected_Implementation(ASelectorBase* InSelector, AActor* InActor)
{
	UPolyMesh_SelectionFunctions::SetMaterialForState(InActor, false, this->OverlayMaterial, this->StencilValue);
}

void ASelectorVisualiserBase::OnSelectorDestroyed(AActor* DestroyedActor)
{
	this->Destroy();
}

