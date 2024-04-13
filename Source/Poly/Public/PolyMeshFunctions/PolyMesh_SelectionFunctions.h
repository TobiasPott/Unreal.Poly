// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PolyMesh_SelectionFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPolyMesh_SelectionFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Visualise|Selection")
	static void SetMaterialForState(AActor* Actor, const bool IsSelected, UMaterialInterface* SelectedMaterial, const uint8 StencilValue = 1);

};
