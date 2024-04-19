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



	// UI Functions
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|Base", meta = (WorldContext = "WorldContext"))
	static bool GetMousePosition(const UObject* WorldContext, const int32 PlayerIndex, FVector2D& OutPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|Base")
	static void GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize);


	UFUNCTION(BlueprintCallable, Category = "Poly|Base")
	static void SelectWithSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD);
	UFUNCTION(BlueprintCallable, Category = "Poly|Base")
	static void DrawRequest(UActorSelectionRequest* Request, AHUD* HUD);

};
