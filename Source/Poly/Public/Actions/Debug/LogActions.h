// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ActionBase.h"
#include "EnumTypes.h"
#include "LogActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API ULogAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	ULogAction() : UActionBase("poly.Log", "Log") {}

	// Members
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString Text = "None";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	EPolyLogCategory LogCategory = EPolyLogCategory::Default;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UPrintAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UPrintAction() : UActionBase("poly.Print", "Print") {}

	// Members
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString Text = "None";


public:
	bool Execute_Implementation(bool bEmitRecord) override;
};
