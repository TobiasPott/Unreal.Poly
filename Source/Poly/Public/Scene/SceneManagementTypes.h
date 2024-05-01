// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneManagementTypes.generated.h"


UCLASS()
class POLY_API USceneNames : public UObject
{
	GENERATED_BODY()

public:
	
	static FName Default;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Scene|Names")
	static void DefaultName(FName& OutName) { OutName = USceneNames::Default; }

};