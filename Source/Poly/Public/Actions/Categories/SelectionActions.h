// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "Selection/SelectorTypes.h"
#include "Modeling/PolyMeshSelection.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "SelectionActions.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class POLY_API UDestroySelectedActorsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDestroySelectedActorsAction()
	{
		Description = "poly.DestroySelectedActors";
		ShortName = "Destroy Selected Actors";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};

UCLASS(Blueprintable)
class POLY_API UDeleteSelectedElementsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDeleteSelectedElementsAction()
	{
		Description = "poly.DeleteSelectedElements";
		ShortName = "Delete Selected Elements";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UFillPolygonAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UFillPolygonAction()
	{
		Description = "poly.FillPolygon";
		ShortName = "Fill Polygon";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};



UCLASS(Blueprintable)
class POLY_API UFlipPolygonAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UFlipPolygonAction()
	{
		Description = "poly.FlipPolygon";
		ShortName = "Flip Polygon";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};