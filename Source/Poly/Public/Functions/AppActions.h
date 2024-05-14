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


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FText Title;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FText Subtext;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString ConfirmLabel = "Yes";
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString DeclineLabel = "No";

	// ToDo: @tpott: GetModalDialog by class, setup (add missing members to base action type) dialog 
	//					and hook Dialog.Closed event to a lambda which calls the action's Submit or Discard
	//					REMEMBER: the boolean return of 'Action.Execute' indicates if action is executed (not success or failure)

public:
	bool Execute_Implementation(bool bEmitRecord) override;
	
	UFUNCTION()
	virtual void OnModalDialogClosed(EModalDialogMessage Message);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnConfirm();
	virtual void OnConfirm_Implementation() { this->Submit(); };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnDecline();
	virtual void OnDecline_Implementation() { this->Discard(); };

};

/**
 * 
 */
UCLASS()
class POLY_API UQuitGameAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UQuitGameAction()
	{
		Description = "poly.QuitGame";
		ShortName = "Quit Game";
	}

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
	// Sets default values for this actor's properties
	UQuitGameWithConfirmAction()
	{
		Description = "poly.QuitGameWithConfirm";
		ShortName = "Quit Game with Confirm";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 PlayerIndex = 0;

public:
	void OnConfirm_Implementation() override;

};