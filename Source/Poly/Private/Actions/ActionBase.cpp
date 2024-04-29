// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/ActionBase.h"
#include "Actions/ActionRef.h"

UActionRef* UActionBase::GetUnique()
{
	UActionRef* Ref = NewObject<UActionRef>(this);
	Ref->Action = this;
	return Ref;
}


