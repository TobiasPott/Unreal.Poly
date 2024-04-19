// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/ActorSelectionRequest.h"
#include "BaseFunctions.generated.h"

/**
 *
 */
UCLASS()
class POLY_API UBaseFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Poly|Base", meta = (WorldContext = "WorldContext", DeterminesOutputType = "InClass", DynamicOutputParam = "OutActor"))
	static void GetOrCreateActor(const UObject* WorldContext, TSubclassOf<AActor> InClass, AActor*& OutActor);



	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities")
	static FTransform Transform_Identity() { return FTransform::Identity; }

};
