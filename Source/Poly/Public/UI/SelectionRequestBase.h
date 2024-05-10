// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/HUD.h"
#include "GenericPlatform/GenericApplication.h"
#include "Misc/App.h"
#include "EngineGlobals.h"
#include "Layout/Margin.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "EngineUtils.h"
#include "Engine/Canvas.h"
#include "Misc/UObjectToken.h"
#include "UObject/UObjectIterator.h"
#include "UI/UserWidgetTypes.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "SelectionRequestBase.generated.h"


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
