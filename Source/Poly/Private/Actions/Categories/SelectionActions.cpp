// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Selection/SelectorBase.h"
#include "Kismet/GameplayStatics.h"
#include "EnumTypes.h"
#include "Modeling/PolyMeshSelection.h"


bool UClearSelectionAction::Execute_Implementation(bool bEmitRecord)
{
	ASelectorBase* Selector;
	if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>()->GetSelector(this, this->SelectorName, Selector))
	{
		UE_LOG(LogPolyTemp, Warning, TEXT("UClearSelectionAction::Execute_Implementation (%s)."), *this->SelectorName.ToString());
		Selector->ClearSelection();
		this->Submit();
		return true;
	}
	this->Discard();
	return false;
}

bool USetSelectionAction::Execute_Implementation(bool bEmitRecord)
{
	// Add poly selection to selector 'Actors'
	ASelectorBase* Selector;
	if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>()->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<UPolySelection*> Selection;
		for (int32 Id : Ids)
			UPolySelection::AddByIdT(Selection, Id);

		UE_LOG(LogPolyTemp, Warning, TEXT("USetSelectionAction::Execute_Implementation: %d/%d (%s)"), Ids.Num(), Selection.Num(), *this->SelectorName.ToString());
		Selector->ReplaceAllT(Selection);
		this->Submit();
		return true;
	}
	this->Discard();
	return false;
}

bool USetElementsSelectionAction::Execute_Implementation(bool bEmitRecord)
{
	// Add poly selection to selector 'Actors'
	ASelectorBase* Selector;
	if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>()->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<UPolyMeshSelection*> Selection;
		for (int32 Id : Ids)
		{
			int32 Index = UPolySelection::AddByIdT(Selection, Id);
			if (Index != INDEX_NONE)
				Selection[Index]->Selection = this->Selections[Index];
		}
		UE_LOG(LogPolyTemp, Warning, TEXT("USetElementsSelectionAction::Execute_Implementation: %d/%d (%s)"), Ids.Num(), Selection.Num(), *this->SelectorName.ToString());
		Selector->ReplaceAllT(Selection);
		this->Submit();
		return true;
	}
	this->Discard();
	return false;
}
