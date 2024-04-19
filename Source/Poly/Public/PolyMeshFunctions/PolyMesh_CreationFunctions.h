// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PolyMeshActor.h"
#include "PolyMesh_CreationFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPolyMesh_CreationFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Poly|Create", meta = (WorldContext = "WorldContext"))
	static void CreateBoxActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset = FVector::ZeroVector, const FVector InDimensions = FVector(100, 100, 100));


};