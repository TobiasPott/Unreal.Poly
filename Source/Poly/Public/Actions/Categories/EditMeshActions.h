// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "Selection/SelectorTypes.h"
#include "EditMeshActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UDeleteMeshElementsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UDeleteMeshElementsAction() : UActionBase("poly.DeleteMeshElements", "Delete Mesh Elements") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UCreatePolygonsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreatePolygonsAction() : UActionBase("poly.CreatePolygons", "Create Polygons") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};



UCLASS(Blueprintable)
class POLY_API UFlipNormalsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UFlipNormalsAction() : UActionBase("poly.FlipNormals", "Flip FlipNormals") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};