// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Debug/LogActions.h"


ULogAction::ULogAction()
{
	Description = "poly.Log";
}

bool ULogAction::Execute_Implementation(bool bEmitRecord)
{
	if (this->LogCategory == EPolyLogCategory::None)
	{
		this->Discard();
		return false;
	}

	switch (this->LogCategory)
	{
	case EPolyLogCategory::Default:
		UE_LOG(LogPoly, Log, TEXT("%s"), *Text);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Text);
		break;
	case EPolyLogCategory::Debug:
		UE_LOG(LogPolyDebug, Log, TEXT("%s"), *Text);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Text);
		break;
	case EPolyLogCategory::Temp:
		UE_LOG(LogPolyTemp, Log, TEXT("%s"), *Text);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Text);
		break;
	}

	this->Submit();
	return true;
}

UPrintAction::UPrintAction()
{
	Description = "poly.Print";
}

bool UPrintAction::Execute_Implementation(bool bEmitRecord)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Text);
	// call base function to fire events
	this->Submit();
	return true;
}
