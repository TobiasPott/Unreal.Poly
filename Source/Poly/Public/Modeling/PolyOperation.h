// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PolySelection.h"
#include "PolyOperation.generated.h"
/**
 * 
 */
UCLASS(BlueprintType)
class POLY_API UPolyOperation : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	UPolyMeshSelection* Selection = nullptr;


public:
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Apply() { };

	// ToDo: @tpott: Ponder if I need or/and can supply a revert function for messh operations
	//				Might be put into a derived class, may include sth to 'preview'

};
