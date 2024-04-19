// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/UserWidgetTypes.h"
#include "UI/ActorSelectionRequest.h"
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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TArray<UActorSelectionRequest*> MarqueeRequests;


public:

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void QueueRequest(UActorSelectionRequest* Request);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ProcessRequests();

};
