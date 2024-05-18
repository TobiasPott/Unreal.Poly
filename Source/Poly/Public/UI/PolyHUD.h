// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/UserWidgetTypes.h"
#include "Selection/SelectionRequest.h"
#include "PolyHUD.generated.h"

/**
 *
 */
UCLASS()
class POLY_API APolyHUD : public AHUD
{
	GENERATED_BODY()

public:
	APolyHUD();

	/** Please add a variable description */	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	bool bProcessRequestsOnDraw =  true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TArray<USelectionRequest*> MarqueeRequests;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TArray<USelectionRequestBase*> BaseRequests;


public:

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void QueueRequest(USelectionRequest* Request);
	UFUNCTION(BlueprintCallable, Category = "Default")
	void QueueBaseRequest(USelectionRequestBase* Request);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ProcessRequests();

	virtual void DrawHUD() override;
};
