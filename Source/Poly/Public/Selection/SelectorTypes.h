// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectorTypes.generated.h"


UENUM(BlueprintType)
enum class ESelectorChannel : uint8 {
	Disabled = 0b00000000,
	Default = 0b00000001,
	Second = 0b00000010,
	Third = 0b00000100,
	Fourth = 0b00001000,
	Fifth = 0b00010000,
	Sixth = 0b00100000,
	Seventh = 0b01000000,
	Eighth = 0b10000000,
};


UCLASS()
class POLY_API USelectorNames : public UObject
{
	GENERATED_BODY()

public:
	static FName Default;
	static FName Second;
	static FName Third;
	static FName Fourth;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selectable")
	void DefaultName(FName& OutName) { OutName = USelectorNames::Default; }
};