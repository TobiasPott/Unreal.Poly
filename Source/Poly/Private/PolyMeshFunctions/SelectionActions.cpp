// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyMeshFunctions/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"


bool UDestroySelectedAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<AActor*> SelectedActors = Selector->Selection;
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			AActor* Selected = SelectedActors[i];
			bool IsSelected;
			Selector->Deselect(Selected, IsSelected);
			Selected->Destroy();
		}
		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}
