// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ToolInterface.h"

bool IToolInterface::CanActivate_Implementation()
{
	return false;
}

bool IToolInterface::Activate_Implementation()
{
	return false;
}

bool IToolInterface::Deactivate_Implementation()
{
	return false;
}

//bool IToolInterface::EmitAction_Implementation(bool bEmit, UActionBase* EmitAction)
//{
//	return false;
//}
