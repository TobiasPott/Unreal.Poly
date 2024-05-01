// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Scene/SceneActor.h"
#include "Scene/SceneManagementTypes.h"
#include "SceneSubsystem.generated.h"

UCLASS()
class ASceneSubsystemRoot : public AActor
{
	GENERATED_BODY()

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> SceneComponent;

public:
	// Sets default values for this actor's properties
	ASceneSubsystemRoot();

};
/**
 * 
 */
UCLASS()
class POLY_API USceneSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:


private:

	bool bIsInitialised = false;

	UPROPERTY()
	ASceneSubsystemRoot* Root;

	UPROPERTY()
	ASceneActor* ActiveScene;

	UPROPERTY()
	TMap<FName, ASceneActor*> Scenes;

protected:

	UFUNCTION()
	ASceneActor* CreateScene(const UObject* WorldContext);


public:

	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	void Init(const UObject* WorldContext);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Scene")
	bool HasScene(FName Name);
	UFUNCTION(BlueprintCallable, Category = "Scene")
	ASceneActor* AddScene(FName Name);
	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	ASceneActor* GetScene(const UObject* WorldContext, FName Name, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Scene", meta = (WorldContext = "WorldContext"))
	ASceneActor* GetActiveScene();

};
