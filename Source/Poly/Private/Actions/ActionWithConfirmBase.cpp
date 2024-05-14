// Fill out your copyright notice in the Description page of Project Settings.
#include "Actions/ActionWithConfirmBase.h"


bool UActionWithConfirmBase::Execute_Implementation(bool bEmitRecord)
{
	if (IsValid(DialogClass))
	{
		UUserWidget* Widget;
		UModalDialogBase::GetModalDialog(this, DialogClass, Widget);
		UModalDialogBase* Dialog = Cast<UModalDialogBase>(Widget);
		if (IsValid(Dialog))
		{
			Dialog->ClearEvents();
			Dialog->Setup(this->Title, this->Subtext, this->ConfirmLabel, this->DeclineLabel);
			Dialog->Closed.AddDynamic(this, &UActionWithConfirmBase::OnModalDialogClosed);
			Dialog->SetVisibility(ESlateVisibility::Visible);
			return true;
		}
	}
	return false;
}

void UActionWithConfirmBase::OnModalDialogClosed(EModalDialogMessage Message)
{
	if (Message == EModalDialogMessage::Confirm)
		this->OnConfirm();
	else if (Message == EModalDialogMessage::Decline)
		this->OnDecline();
}
