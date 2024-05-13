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


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI", meta = (WorldContext = "WorldContext"))
	static bool GetScreenRay(const UObject* WorldContext, const int32 PlayerIndex, const FVector2D& ScreenPosition, FVector& WorldPosition, FVector& WorldDirection);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI", meta = (WorldContext = "WorldContext"))
	static bool GetScreenRaySegment(const UObject* WorldContext, const int32 PlayerIndex, const FVector2D& ScreenPosition, FVector& WorldStart, FVector& WorldEnd, const float Distance = 10000);
	
	UFUNCTION(BlueprintCallable, Category = "Poly|UI", meta = (WorldContext = "WorldContext"))
	static void GetScreenRectWorldCorners(const UObject* WorldContext, const int32 PlayerIndex, const FVector2D& BottomLeft, const FVector2D& BottomRight, const FVector2D& TopRight, const FVector2D& TopLeft, TArray<FVector>& OutNearCorners, TArray<FVector>& OutFarCorners, const float Distance = 10000);

	/**
	* Gets the Origin and Size of the rectangle described by the given points. The Origin is the upper left corner and size the absolute extents
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI")
	static void GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Poly|UI")
	static void GetRectCorners(const FVector2D Origin, const FVector2D Size, FVector2D& OutBottomLeft, FVector2D& OutBottomRight, FVector2D& OutTopRight, FVector2D& OutTopLeft);
	


	UFUNCTION(BlueprintCallable, Category = "Poly|UI")
	static void SelectWithSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD);

	UFUNCTION(BlueprintCallable, Category = "Poly|UI")
	static void DrawRequest(USelectionRequestBase* Request, AHUD* HUD, const FLinearColor& RectColor = FLinearColor(1, 1, 1, 0.25));
};
