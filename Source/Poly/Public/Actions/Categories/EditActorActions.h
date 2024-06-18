// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "Gizmos/GizmoTypes.h"
#include "Selection/SelectorTypes.h"
#include "EditActorActions.generated.h"


/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UDestroyActorsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UDestroyActorsAction() : UActionBase("poly.DestroyActors", "Destroy Actors") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};

UCLASS(Blueprintable)
class POLY_API UTransformSelectionAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UTransformSelectionAction() : UActionBase("poly.TransformSelection", "Transform Selection") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FTransform DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	ETransformSpace Space = ETransformSpace::TS_World;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};
