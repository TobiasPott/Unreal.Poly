// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "Modeling/PolySelection.h"
#include "Modeling/PolyMeshSelection.h"
#include "Poly_MeshAnalysisFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPoly_MeshAnalysisFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Geometry Script|Selection")
	static bool GetCenterOfSelectionBounds(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter);



	UFUNCTION(BlueprintCallable, Category = "Geometry Script|Analysis")
	static bool GetSelectionMormal(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutNormal, FVector& OutTangent);


};
