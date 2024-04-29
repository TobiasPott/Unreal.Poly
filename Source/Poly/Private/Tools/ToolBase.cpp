// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ToolBase.h"

void UToolBase::OnActivated()
{
	if (Activated.IsBound())
		Activated.Broadcast(this);
}

void UToolBase::OnDeactivated()
{
	if (Deactivated.IsBound())
		Deactivated.Broadcast(this);
}

bool UToolBase::Activate_Implementation()
{
	return false;
}

bool UToolBase::Deactivate_Implementation()
{
	return false;
}

bool UToolBase::EmitAction(bool bEmit, UActionBase* EmitAction)
{
	if (ActionEmitted.IsBound())
		ActionEmitted.Broadcast(this, bEmit, EmitAction);
	return bEmit;
}
