// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionRef.h"

int32 UActionRef::InstanceCount = 0;

void UActionRef::Init(UActionBase* InAction)
{
	if (IsValid(Action))
	{
		this->Action = InAction;
	}
}

UActionRef::UActionRef()
{
	Id = InstanceCount++;
	if (InstanceCount >= MAX_int32)
		InstanceCount = MIN_int32;
}

bool UActionRef::Execute_Implementation(bool bSilent, bool bEmitRecord)
{
	bool bExecuted = false;
	if (IsValid(Action))
	{
		bExecuted = Action->Execute(bEmitRecord);
	}
	if (!bSilent)
	{
		if (bExecuted) this->Submit();
		else this->Discard();
	}
	return bExecuted;
}

UActionRef* UActionRef::GetUnique()
{
	UActionRef* Ref = NewObject<UActionRef>(Action);
	Ref->Action = this;
	return Ref;
}

