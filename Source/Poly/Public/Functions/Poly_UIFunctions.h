// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/ActorSelectionRequest.h"
#include "UI/SelectionRequestBase.h"
#include "Poly_UIFunctions.generated.h"

/**
 *
 */
UCLASS()
class POLY_API UPoly_UIFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// UI Functions
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI", meta = (WorldContext = "WorldContext"))
	static bool GetMousePosition(const UObject* WorldContext, const int32 PlayerIndex, FVector2D& OutPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI", meta = (WorldContext = "WorldContext"))
	static bool GetMouseRay(const UObject* WorldContext, const int32 PlayerIndex, FVector& WorldPosition, FVector& WorldDirection);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI", meta = (WorldContext = "WorldContext"))
	static bool GetMouseRaySegment(const UObject* WorldContext, const int32 PlayerIndex, FVector& WorldStart, FVector& WorldEnd, const float Distance = 10000);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI")
	static void GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize);


	UFUNCTION(BlueprintCallable, Category = "Poly|UI")
	static void SelectWithSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD);

	UFUNCTION(BlueprintCallable, Category = "Poly|UI")
	static void DrawRequest(USelectionRequestBase* Request, AHUD* HUD);

};
