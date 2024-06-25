// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ActionBase.h"
#include "Actions/ActionWithConfirmBase.h"
#include "AppActions.generated.h"


/**
 * 
 */
UCLASS()
class POLY_API UQuitGameAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UQuitGameAction() : UActionBase("poly.QuitGame", "Quit Game") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 PlayerIndex = 0;

public:
	bool Execute_Implementation(bool bEmitRecord) override;

};

UCLASS()
class POLY_API UQuitGameWithConfirmAction : public UActionWithConfirmBase
{
	GENERATED_BODY()

public:
	// Ctor
	UQuitGameWithConfirmAction() : UActionWithConfirmBase("poly.QuitGameWithConfirm", "Quit Game with Confirm") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 PlayerIndex = 0;

public:
	void OnConfirm_Implementation() override;

};