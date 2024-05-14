// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ModalDialogBase.h"
#include "Actions/ActionBase.h"
#include "ActionWithConfirmBase.generated.h"

/**
 * 
 */
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
