// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IdentifierComponent.h"
#include "Poly_IdentifierFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPoly_IdentifierFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static bool GetActorById(const int32 IdentifierId, AActor*& OutActor, UIdentifierComponent*& OutComponent);

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static bool GetActorId(AActor* Actor, int32& OutId, UIdentifierComponent*& OutComponent, const bool bAdd = false);


};
