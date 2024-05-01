// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Scene/SceneManagementTypes.h"
#include "SceneSubsystemFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API USceneSubsystemFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:


	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	static void AddToScene(const UObject* WorldContext, AActor* InActor, const FName InSceneName = "Default");
	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	static void AddToActiveScene(const UObject* WorldContext, AActor* InActor);


	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	static void RemoveFromScene(const UObject* WorldContext, AActor* InActor, const FName InSceneName = "Default");
	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	static void RemoveFromActiveScene(const UObject* WorldContext, AActor* InActor);
};
