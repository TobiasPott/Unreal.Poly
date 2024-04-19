// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "PolyMesh_CreationActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UCreateBoxAction: public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreateBoxAction()
	{
		Description = "poly.CreateBox";
		ShortName = "Create Box";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Actions|Creation", meta = (ExposeOnSpawn = "true"))
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Actions|Creation", meta = (ExposeOnSpawn = "true"))
	FVector Dimensions = FVector(100, 100, 100);

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};