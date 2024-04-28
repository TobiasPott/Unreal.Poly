// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Selection/SelectorBase.h"
#include "Selection/SelectorVisualiserBase.h"
#include "Selection/SelectorTypes.h"
#include "SelectorSubsystem.generated.h"


/** Please add a class description */
UCLASS()
class ASelectorSubsystemRoot : public AActor
{
	GENERATED_BODY()

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> SceneComponent;

public:
	// Sets default values for this actor's properties
	ASelectorSubsystemRoot();

};

/**
 * 
 */
UCLASS()
class POLY_API USelectorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:


private:
	UPROPERTY()
	ASelectorSubsystemRoot* SubsystemRoot;

	UPROPERTY()
	TMap<FName, ASelectorBase*> Selectors;

protected:

	UFUNCTION()
	ASelectorBase* CreateSelector(const UObject* WorldContext);


public:

	UFUNCTION(BlueprintCallable, Category = "Selector", meta = (WorldContext = "WorldContext"))
	void Init(const UObject* WorldContext);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Selector")
	bool HasSelector(FName Name);
	UFUNCTION(BlueprintCallable, Category = "Selector")
	ASelectorBase* AddSelector(FName Name, uint8 Stencil = 1);
	UFUNCTION(BlueprintCallable, Category = "Selector", meta = (WorldContext = "WorldContext"))
	bool GetSelector(const UObject* WorldContext, FName Name, ASelectorBase*& OutSelector);

};
