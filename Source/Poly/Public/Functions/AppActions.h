// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ActionBase.h"
#include "AppActions.generated.h"

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