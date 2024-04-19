// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/ActorSelectionRequest.h"
#include "UIFunctions.generated.h"

/**
 *
 */
UCLASS()
class POLY_API UUIFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// UI Functions
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|Base", meta = (WorldContext = "WorldContext"))
	static bool GetMousePosition(const UObject* WorldContext, const int32 PlayerIndex, FVector2D& OutPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|Base")
	static void GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize);


	UFUNCTION(BlueprintCallable, Category = "Poly|Base")
	static void SelectWithRectSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD);
	UFUNCTION(BlueprintCallable, Category = "Poly|Base")
	static void DrawRequest(UActorSelectionRequest* Request, AHUD* HUD);

};
