// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionBase.h"

UPrintAction::UPrintAction()
{
	Description = "poly.Print";
}

void UPrintAction::Execute(bool bEmitRecord)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Text)
}
