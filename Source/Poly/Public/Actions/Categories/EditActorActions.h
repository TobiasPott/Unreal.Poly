// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
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
