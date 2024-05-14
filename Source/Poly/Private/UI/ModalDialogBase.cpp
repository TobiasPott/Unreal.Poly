// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ModalDialogBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UModalDialogBase::ClearEvents()
{
	this->Closed.Clear();
}



void UModalDialogBase::GetModalDialog(UObject* WorldContext, TSubclassOf<UModalDialogBase> InClass, UUserWidget*& Widget)
{
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContext, Widgets, InClass, true);
	if (!Widgets.IsEmpty())
	{
		Widget = Widgets[0];
		if (!Widget->IsInViewport())
			Widget->AddToViewport(65536);
		return;
	}
	else
	{
		Widget = CreateWidget<UUserWidget>(WorldContext->GetWorld(), InClass);
		Widget->AddToViewport(65536);
		return;
	}
}
