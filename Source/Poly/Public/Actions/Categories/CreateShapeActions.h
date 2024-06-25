// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "CreateShapeActions.generated.h"


/**
 *
 */
UCLASS()
class POLY_API UCreateShapeActionBase : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateShapeActionBase() : UActionBase("poly.Light.CreateShapeActionBase", "<CreateShapeActionBase>") {}
	UCreateShapeActionBase(const FString& InDescription, const FString& InShortName) : UActionBase("poly.Shapes." + InDescription, InShortName) {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FVector Offset = FVector::ZeroVector;
};

UCLASS(Blueprintable)
class POLY_API UCreateBoxAction : public UCreateShapeActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateBoxAction() : UCreateShapeActionBase("CreateBox", "Create Box") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FVector Dimensions = FVector(100, 100, 100);

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UCreateSphereAction : public UCreateShapeActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateSphereAction() : UCreateShapeActionBase("CreateSphere", "Create Sphere") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float Radius = 50.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 StepX = 6;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 StepY = 6;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 StepZ = 6;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UCreateCylinderAction : public UCreateShapeActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateCylinderAction() : UCreateShapeActionBase("CreateCylinder", "Create Cylinder") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float Radius = 50.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float Height = 100.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 RadialSteps = 12;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 HeightSteps = 0;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UCreateConeAction : public UCreateShapeActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreateConeAction() : UCreateShapeActionBase("CreateCone", "Create Cone") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float BaseRadius = 50.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float TopRadius = 5.0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float Height = 100;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 RadialSteps = 12;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 HeightSteps = 4;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};
