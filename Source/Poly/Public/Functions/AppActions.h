// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ActionBase.h"
#include "UI/ModalDialogBase.h"
#include "AppActions.generated.h"



UCLASS()
class POLY_API UActionWithConfirmBase : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UActionWithConfirmBase()
	{
		Description = "poly.ActionWithConfirm";
		ShortName = "<Action With Confirm>";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	TSubclassOf<UModalDialogBase> DialogClass = UModalDialogBase::StaticClass();

	// ToDo: @tpott: GetModalDialog by class, setup (add missing members to base action type) dialog 
	//					and hook Dialog.Closed event to a lambda which calls the action's Submit or Discard
	//					REMEMBER: the boolean return of 'Action.Execute' indicates if action is executed (not ssuccess or failure)
};

/**
 * 
 */
UCLASS()
class POLY_API UQuitGameActions : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UQuitGameActions()
	{
		Description = "poly.QuitGame";
		ShortName = "Quit Game";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 PlayerIndex = 0;

public:
	bool Execute_Implementation(bool bEmitRecord) override;

};