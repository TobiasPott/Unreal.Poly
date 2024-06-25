// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionRef.h"

int32 UActionRef::InstanceCount = 0;

UActionRef::UActionRef()
{
	Id = InstanceCount++;
	if (InstanceCount >= MAX_int32)
		InstanceCount = MIN_int32;
}

void UActionRef::Init(UActionBase* InAction)
{
	if (IsValid(Action))
	{
		this->Action = InAction;
	}
}


bool UActionRef::Execute_Implementation(bool bEmitRecord)
{
	bool bExecuted = false;
	if (IsValid(Action))
	{
		bExecuted = Action->Execute(bEmitRecord);
	}
	if (bExecuted) this->Submit();
	else this->Discard();
	return bExecuted;
}

UActionRef* UActionRef::GetUnique()
{
	UActionRef* Ref = NewObject<UActionRef>(Action);
	Ref->Action = this;
	return Ref;
}

