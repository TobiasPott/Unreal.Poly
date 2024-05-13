// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GizmoTypes.h"
#include "GizmoFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UGizmoFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gizmo Pivot")
	static FVector GetLocation(const FGizmoPivot& Pivot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gizmo Pivot")
	static FRotator GetOrientation(const FGizmoPivot& Pivot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gizmo Pivot")
	static ETransformSpace GetSpace(const FGizmoPivot& Pivot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gizmo Pivot")
	static ETransformSpace GetInverseSpace(const FGizmoPivot& Pivot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gizmo Pivot")
	static bool IsLocalSpace(const FGizmoPivot& Pivot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gizmo Pivot")
	static bool IsWorldSpace(const FGizmoPivot& Pivot);
};
