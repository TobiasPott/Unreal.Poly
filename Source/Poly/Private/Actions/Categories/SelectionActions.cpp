// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Selection/SelectorBase.h"
#include "Kismet/GameplayStatics.h"
#include "EnumTypes.h"

bool USetSelectionAction::Execute_Implementation(bool bEmitRecord)
{
	// Add poly selection to selector 'Actors'
	ASelectorBase* Selector;
	if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>()->GetSelector(this, this->SelectorName, Selector))
	{
		// ToDo: add Ids to Selectoor
		TArray<UPolySelection*> Selection;
		for (int32 Id : Ids)
		{
			UPolySelection::AddByIdT(Selection, Id);
		}
		Selector->ReplaceAllT(Selection);
		UE_LOG(LogPolyTemp, Warning, TEXT("USetSelectionAction::Execute_Implementation: %d/%d"), Ids.Num(), Selection.Num());
		this->Submit();
		return true;
	}
	this->Discard();
	return false;
}
