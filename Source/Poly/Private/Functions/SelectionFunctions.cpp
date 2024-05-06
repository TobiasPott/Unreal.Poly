// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/SelectionFunctions.h"
#include "Components/BaseDynamicMeshComponent.h"

void UPoly_SelectionFunctions::SetMaterialForState(AActor* Actor, const bool IsSelected, UMaterialInterface* SelectedMaterial, const uint8 StencilValue)
{
	if (!IsValid(Actor))
		return;

	// dertemine material to use
	UMaterialInterface* Material = IsSelected ? SelectedMaterial : nullptr;

	TInlineComponentArray<UActorComponent*> Components;
	Actor->GetComponents(Components); //s (UMeshComponent::StaticClass());
	for (int i = 0; i < Components.Num(); i++)
	{
		UActorComponent* Comp = Components[i];
		UMeshComponent* MeshComp = Cast<UMeshComponent>(Comp);
		if (IsValid(MeshComp))
		{
			MeshComp->SetOverlayMaterial(Material);
			MeshComp->SetRenderCustomDepth(IsSelected);
			MeshComp->SetCustomDepthStencilValue(IsSelected ? StencilValue : 0);

			UBaseDynamicMeshComponent* DynMeshComp = Cast<UBaseDynamicMeshComponent>(Comp);
			if (IsValid(DynMeshComp))
				DynMeshComp->SetOverrideRenderMaterial(Material);
		}
	}
}
