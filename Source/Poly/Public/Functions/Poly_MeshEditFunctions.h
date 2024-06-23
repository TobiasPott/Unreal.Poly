// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Gizmos/GizmoTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "UDynamicMesh.h"
#include "Poly_MeshEditFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPoly_MeshEditFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public: 
	static void AddMeshElementsTransform(UDynamicMesh* TargetMesh, FGeometryScriptMeshSelection Selection, FVector InOffset, FQuat InRotation, FVector InScale, ETransformSpace Space = ETransformSpace::TS_World);
	static void AddMeshElementsTransform(UDynamicMesh* TargetMesh, FGeometryScriptMeshSelection Selection, FTransform InTransform, ETransformSpace Space = ETransformSpace::TS_World);
	//static void AddTransforms(const TArray<AActor*> Actors, FTransform InTransform, ETransformSpace Space = ETransformSpace::TS_World);


};
