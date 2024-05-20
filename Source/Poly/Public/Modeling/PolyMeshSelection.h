// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "Modeling/PolySelection.h"
#include "PolyMeshSelection.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class POLY_API UPolyMeshSelection : public UPolySelection
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UDynamicMesh* TargetMesh = nullptr;


public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FGeometryScriptMeshSelection Selection = FGeometryScriptMeshSelection();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FTransform LocalToWorld = FTransform::Identity;


public:
	virtual bool Resolve_Implementation() override;
};
