// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ToolBaseActor.h"


void AToolBaseActor::OnToolActivated(UToolBase* InTool)
{
	if (ToolActivated.IsBound())
		ToolActivated.Broadcast(InTool);
}

void AToolBaseActor::OnToolDeactivated(UToolBase* InTool)
{
	if (IsValid(InTool))
	{
		if (ToolDeactivated.IsBound())
			ToolDeactivated.Broadcast(InTool);

		Tool->Activated.AddDynamic(this, &AToolBaseActor::OnToolActivated);
		Tool->Deactivated.AddDynamic(this, &AToolBaseActor::OnToolActivated);
	}
}

bool AToolBaseActor::Activate_Implementation()
{
	if (IsValid(Tool))
		return Tool->Activate();
	return false;
}

bool AToolBaseActor::Deactivate_Implementation()
{
	if (IsValid(Tool))
		return Tool->Deactivate();
	return false;
}

void AToolBaseActor::SetTool(UToolBase* NewTool)
{
	if (!IsValid(Tool)
		&& IsValid(NewTool))
	{
		Tool = NewTool;
		Tool->Activated.AddDynamic(this, &AToolBaseActor::OnToolActivated);
		Tool->Deactivated.AddDynamic(this, &AToolBaseActor::OnToolActivated);
	}
}
