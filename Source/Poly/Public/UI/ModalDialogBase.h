// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "ModalDialogBase.generated.h"

UENUM(BlueprintType)
enum class EModalDialogMessage : uint8 {
	None = 0,
	Confirm,
	Decline,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModalDialogEvent, EModalDialogMessage, Message);

/**
 * 
 */
UCLASS()
class POLY_API UModalDialogBase : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ClearEvents();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Default")
	void Setup(const FText& InTitle, const FText& InSubtext, const FString& InConfirmLabel = "Yes", const FString& InDeclineLabel = "No");
	//virtual void Setup_Implementation(const FText& InTitle, const FText& InSubtext, const FString& InConfirmLabel = "Yes", const FString& InDeclineLabel = "No");


	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default")
	FModalDialogEvent Closed;





public:

	UFUNCTION(BlueprintCallable, Category = "Poly|UI", meta = (WorldContext = "WorldContext", DeterminesOutputType = "InClass", DynamicOutputParam = "Widget"))
	static void GetModalDialog(UObject* WorldContext, TSubclassOf<UModalDialogBase> InClass, UUserWidget*& Widget);

};
