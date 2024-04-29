// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionCollection.h"


/**
*	UActionCollection
*/
void UActionCollection::AddItems(TArray<UActionBase*> InActions)
{
	uint8 NewIndent = this->bIsRoot ? this->Indent : this->Indent + 1;
	for (int i = 0; i < InActions.Num(); i++)
	{
		InActions[i]->SetIndent(NewIndent);
		this->Actions.AddUnique(InActions[i]);
	}
}

void UActionCollection::AddItem(UActionBase* InAction)
{
	uint8 NewIndent = this->bIsRoot ? this->Indent : this->Indent + 1;
	InAction->SetIndent(NewIndent);
	this->Actions.AddUnique(InAction);
}

void UActionCollection::SetIndent(const uint8 InIndent)
{
	Super::SetIndent(InIndent);

	uint8 NewIndent = this->bIsRoot ? InIndent : InIndent + 1;
	for (int i = 0; i < Actions.Num(); i++)
		Actions[i]->SetIndent(NewIndent);
}
