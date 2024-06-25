// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "CreateLightActions.generated.h"


/**
 *
 */
UCLASS()
class POLY_API UCreateLightActionBase : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateLightActionBase() : UActionBase("poly.Light.CreateLightActionBase", "<CreateLightActionBase>") {}
	UCreateLightActionBase(const FString& InDescription, const FString& InShortName) : UActionBase("poly.Light." + InDescription, InShortName) {}
};



UCLASS(Blueprintable)
class POLY_API UCreateDirectionalLightAction : public UCreateLightActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateDirectionalLightAction() : UCreateLightActionBase("CreateDirectionalLight", "Create Directional Light") {}

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};

UCLASS(Blueprintable)
class POLY_API UCreateSpotLightAction : public UCreateLightActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateSpotLightAction() : UCreateLightActionBase("CreateSpotLight", "Create Spot Light") {}

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};

UCLASS(Blueprintable)
class POLY_API UCreatePointLightAction : public UCreateLightActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreatePointLightAction() : UCreateLightActionBase("CreatePointLight", "Create Point Light") {}

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};
