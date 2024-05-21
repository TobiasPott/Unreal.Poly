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
	Actors = 0b01000000,
	Elements = 0b10000000,
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
	static FName Actors;
	static FName Elements;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selector|Names")
	static void DefaultName(FName& OutName) { OutName = USelectorNames::Default; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selector|Names")
	static void SecondName(FName& OutName) { OutName = USelectorNames::Second; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selector|Names")
	static void ThirdName(FName& OutName) { OutName = USelectorNames::Third; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selector|Names")
	static void FourthName(FName& OutName) { OutName = USelectorNames::Fourth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selector|Names")
	static void MeshName(FName& OutName) { OutName = USelectorNames::Actors; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selector|Names")
	static void ElementshName(FName& OutName) { OutName = USelectorNames::Elements; }
};