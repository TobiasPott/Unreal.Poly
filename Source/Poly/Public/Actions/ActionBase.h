// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionBase.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UActionBase : public UObject
{
	GENERATED_BODY()

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	uint8 Category = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 Target = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString Description = "poly.Action";


public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	virtual void Execute(bool bEmitRecord = true) { };

};


UCLASS(Blueprintable)
class POLY_API UPrintAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UPrintAction();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString Text = "None";


public:
	void Execute(bool bEmitRecord) override;
};
