// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/ActionBase.h"
#include "Actions/ActionRef.h"

UActionRef* UActionBase::GetUnique()
{
	UActionRef* Ref = NewObject<UActionRef>(this);
	Ref->Action = this;
	return Ref;
}



/**
*	UActionCollection
*/
void UActionCollection::AddItems(TArray<UActionBase*> InActions)
{
	for (int i = 0; i < InActions.Num(); i++)
		this->Actions.AddUnique(InActions[i]);
}

void UActionCollection::AddItem(UActionBase* InAction)
{
	this->Actions.AddUnique(InAction);
}
