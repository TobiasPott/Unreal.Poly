// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/AppActions.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actions/ActionMacros.h"



bool UQuitGameActions::Execute_Implementation(bool bEmitRecord)
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, this->PlayerIndex), EQuitPreference::Quit, false);
	RETURN_ACTIONSUBMIT_ALWAYS()
}
