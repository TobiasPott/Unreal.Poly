// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "PolyMeshSelection.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class POLY_API UPolyMeshSelection : public UObject
{
	GENERATED_BODY()
	

public:


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	UDynamicMesh* Target = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FGeometryScriptMeshSelection Selection = FGeometryScriptMeshSelection();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FTransform LocalToWorld = FTransform::Identity;



};
