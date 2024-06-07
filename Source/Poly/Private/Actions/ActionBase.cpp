// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/ActionBase.h"
#include "Actions/ActionRef.h"
#include "Actions/ActionCollection.h"

UActionRef* UActionBase::GetUnique()
{
	UActionRef* Ref = NewObject<UActionRef>(this);
	Ref->Action = this;
	return Ref;
}

void UActionBase::CreateActionCollection(UObject* WorldContext, const FString InShortName, const FString InDescription, TArray<UActionBase*>& InActions, UActionCollection*& OutCollection)
{
	OutCollection = NewObject<UActionCollection>(WorldContext);
	OutCollection->SetShortNameAndDescription(InShortName, InDescription);
	OutCollection->AddItems(InActions);
}


