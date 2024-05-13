// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "Selection/SelectorTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "SelectionActions.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class POLY_API UDestroySelectedAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDestroySelectedAction()
	{
		Description = "poly.DestroySelected";
		ShortName = "Destroy Selected";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Actions|Selection", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Default;

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

	UPROPERTY()
	AActor* Target;

	UPROPERTY()
	FGeometryScriptMeshSelection Selection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Actions|Selection", meta = (ExposeOnSpawn = "true"))
	bool bCompact = false;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};