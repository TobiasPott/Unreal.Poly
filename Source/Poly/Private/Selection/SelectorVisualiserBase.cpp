// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorVisualiserBase.h"
#include "Selection/SelectorBase.h"
#include "Functions/Poly_SelectorFunctions.h"
#include "Functions/Poly_ActorFunctions.h"

// Sets default values
ASelectorVisualiserBase::ASelectorVisualiserBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = UPoly_ActorFunctions::CreateDefaultSceneComponent<USceneComponent>(this, "DefaultSceneRoot", EComponentMobility::Static);

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
		Selector->SelectableSelected.AddDynamic(this, &ASelectorVisualiserBase::OnSelectableSelected);
		Selector->SelectableDeselected.AddDynamic(this, &ASelectorVisualiserBase::OnSelectableDeselected);
		Selector->OnDestroyed.AddDynamic(this, &ASelectorVisualiserBase::OnSelectorDestroyed);
		bIsInitialised = true;
	}
}

void ASelectorVisualiserBase::OnSelectableSelected_Implementation(ASelectorBase* InSelector, UPolySelection* InSelectable)
{
	if (IsValid(InSelectable->GetSelectedActor()))
		UPoly_SelectorFunctions::SetMaterialForState(InSelectable->GetSelectedActor()->GetOwner(), true, this->OverlayMaterial, InSelector->Stencil);
}

void ASelectorVisualiserBase::OnSelectableDeselected_Implementation(ASelectorBase* InSelector, UPolySelection* InSelectable)
{
	if (IsValid(InSelectable->GetSelectedActor()))
		UPoly_SelectorFunctions::SetMaterialForState(InSelectable->GetSelectedActor()->GetOwner(), false, this->OverlayMaterial, InSelector->Stencil);
}

void ASelectorVisualiserBase::OnSelectorDestroyed(AActor* DestroyedActor)
{
	this->Destroy();
}

