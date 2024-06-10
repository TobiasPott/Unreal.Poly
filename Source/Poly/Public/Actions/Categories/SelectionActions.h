// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "GeometryScript/MeshModelingFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "Selection/SelectorTypes.h"
#include "SelectionActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API USetSelectionAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	USetSelectionAction() : UActionBase("poly.SetSelection", "Set Selection") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TSet<int32> Ids;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};
