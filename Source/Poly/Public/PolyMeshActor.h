// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "PolyMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API APolyMeshActor : public ADynamicMeshActor
{
	GENERATED_BODY()

public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetCollision(bool bIsEnabled);

};
