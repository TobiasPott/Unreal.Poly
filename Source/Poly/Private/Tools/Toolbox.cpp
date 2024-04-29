// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Toolbox.h"

// Sets default values
AToolbox::AToolbox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

bool AToolbox::SetActive(TScriptInterface<IToolInterface> InTool)
{
	if (IsValid(ActiveTool.GetObject()))
	{
		ActiveTool->Deactivate();
		ActiveTool = nullptr;
	}

	if (IsValid(InTool.GetObject()))
	{
		ActiveTool = InTool;
		InTool->Activate();
	}
	return false;
}

