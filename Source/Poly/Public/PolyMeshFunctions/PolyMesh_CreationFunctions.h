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
	static void CreateBoxActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset = FVector::ZeroVector, 
		const FVector InDimensions = FVector(100, 100, 100));
	
	UFUNCTION(BlueprintCallable, Category = "Poly|Create", meta = (WorldContext = "WorldContext"))
	static void CreateSphereActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset = FVector::ZeroVector, 
		const float Radius = 50.0f, int32 StepsX = 6, int32 StepsY = 6, int32 StepsZ = 6);

	UFUNCTION(BlueprintCallable, Category = "Poly|Create", meta = (WorldContext = "WorldContext"))
	static void CreateCylinderActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset = FVector::ZeroVector,
		const float Radius = 50.0f, const float Height = 100.0f, const int32 RadialSteps = 12, const int32 HeightSteps = 0);
	
	UFUNCTION(BlueprintCallable, Category = "Poly|Create", meta = (WorldContext = "WorldContext"))
	static void CreateConeActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset = FVector::ZeroVector,
		const float BaseRadius = 50, const float TopRadius = 5, const float Height = 100, const int32 RadialSteps = 12, const int32 HeightSteps = 4);

};