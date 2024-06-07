// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/AppActions.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actions/ActionMacros.h"



bool UQuitGameAction::Execute_Implementation(bool bEmitRecord)
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, this->PlayerIndex), EQuitPreference::Quit, false);
	RETURN_ACTIONSUBMIT_ALWAYS()
}

void UQuitGameWithConfirmAction::OnConfirm_Implementation()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, this->PlayerIndex), EQuitPreference::Quit, false);
	Super::OnConfirm_Implementation();
}
