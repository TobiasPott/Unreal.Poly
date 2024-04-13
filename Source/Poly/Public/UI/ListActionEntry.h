// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ActionBase.h"
#include "ListActionEntry.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class POLY_API UListActionEntry : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UActionBase> Action = nullptr;
};
