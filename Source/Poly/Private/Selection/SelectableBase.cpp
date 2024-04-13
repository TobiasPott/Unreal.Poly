// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectableBase.h"
#include "Selection/SelectorBase.h"
#include "BaseFunctions.h"
#include "Components/BaseDynamicMeshComponent.h"

// Sets default values for this component's properties
USelectableBase::USelectableBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USelectableBase::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnClicked.AddDynamic(this, &USelectableBase::OnActorClicked);
}


void USelectableBase::OnActorClicked_Implementation(AActor* TouchedActor, FKey ButtonPressed)
{
	ASelectorBase* Selector;
	GetSelector(Selector);

	AActor* Actor = this->GetOwner();
	if (Selector->IsSelected(Actor))
	{
		bool bIsSelected = false;
		Selector->Deselect(Actor, bIsSelected);
		this->ChangeState(bIsSelected);
	}
	else
	{
		bool bIsSelected = false;
		Selector->Select(Actor, bIsSelected);
		this->ChangeState(bIsSelected);
	}

	UE_LOG(LogTemp, Warning, TEXT("Actor.Clicked: %s"), *TouchedActor->GetName())
}

void USelectableBase::ChangeState_Implementation(const bool bIsSelected)
{

}


void USelectableBase::GetSelector_Implementation(ASelectorBase*& OutActor)
{
	AActor* Actor;
	UBaseFunctions::GetOrCreateActor(this->GetWorld(), ASelectorBase::StaticClass(), Actor);
	OutActor = Cast<ASelectorBase>(Actor);
}

void USelectableBase::SetMaterialForState_Implementation(bool IsSelected, UMaterialInterface* SelectedMaterial)
{
	// dertemine material to use
	UMaterialInterface* Material = IsSelected ? SelectedMaterial : nullptr;

	TArray<UActorComponent*> Components = this->GetOwner()->GetComponentsByClass(UMeshComponent::StaticClass());
	for (int i = 0; i < Components.Num(); i++)
	{
		UActorComponent* Comp = Components[i];
		UMeshComponent* MeshComp = Cast<UMeshComponent>(Comp);
		if (IsValid(MeshComp))
		{
			MeshComp->SetOverlayMaterial(Material);
			MeshComp->SetRenderCustomDepth(IsSelected);
			MeshComp->SetCustomDepthStencilValue(IsSelected ? 1 : 0);

			UBaseDynamicMeshComponent* DynMeshComp = Cast<UBaseDynamicMeshComponent>(Comp);
			if (IsValid(DynMeshComp))
				DynMeshComp->SetOverrideRenderMaterial(Material);
		}
	}
}
