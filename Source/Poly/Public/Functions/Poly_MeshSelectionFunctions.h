// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "Poly_MeshSelectionFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPoly_MeshSelectionFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Geometry Script|Selection")
	static bool GetSelectionCenterOfBounds(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter);
	//UFUNCTION(BlueprintCallable, Category = "Geometry Script|Selection")
	//static bool GetSelectionCenter(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter);
};
