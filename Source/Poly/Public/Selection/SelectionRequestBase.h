// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetTypes.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "SelectionRequestBase.generated.h"

/**
 * Type of combine behaviour for actor selections
 */
UENUM(BlueprintType)
enum class EPolySelectionMode : uint8
{
	Replace = 0,
	Select = 1,
	Deselect = 2,
};


UCLASS(Blueprintable)
class POLY_API USelectionRequestBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	bool bSubmitted = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	ESelectionRequestMode Mode = ESelectionRequestMode::Click;

	UPROPERTY()
	FVector2D FirstPoint = FVector2D(0, 0);
	UPROPERTY()
	FVector2D SecondPoint = FVector2D(0, 0);


	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Submit() { this->bSubmitted = true; }

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void UpdateSecondPoint(const FVector2D InSecondPoint) { this->SecondPoint = InSecondPoint; }

};
