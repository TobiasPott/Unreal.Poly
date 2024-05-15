// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "CreationActions.generated.h"



UCLASS()
class POLY_API UCreateShapeActionBase : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreateShapeActionBase()
	{
		Description = "poly.CreateShape";
		ShortName = "<Create Shape>";
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FVector Offset = FVector::ZeroVector;
};

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UCreateBoxAction: public UCreateShapeActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreateBoxAction()
	{
		Description = "poly.CreateBox";
		ShortName = "Create Box";
	}

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
	// Sets default values for this actor's properties
	UCreateSphereAction()
	{
		Description = "poly.CreateSphere";
		ShortName = "Create Sphere";
	}

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
	// Sets default values for this actor's properties
	UCreateCylinderAction()
	{
		Description = "poly.CreateCylinder";
		ShortName = "Create Cylinder";
	}

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
	// Sets default values for this actor's properties
	UCreateConeAction()
	{
		Description = "poly.CreateCone";
		ShortName = "Create Cone";
	}

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














UCLASS(Blueprintable)
class POLY_API UCreateDirectionalLightAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreateDirectionalLightAction()
	{
		Description = "poly.CreateDirectionalLight";
		ShortName = "Create Directional Light";
	}
public:
	bool Execute_Implementation(bool bEmitRecord) override;
};

UCLASS(Blueprintable)
class POLY_API UCreateSpotLightAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreateSpotLightAction()
	{
		Description = "poly.CreateSpotLight";
		ShortName = "Create Spot Light";
	}
public:
	bool Execute_Implementation(bool bEmitRecord) override;
};

UCLASS(Blueprintable)
class POLY_API UCreatePointLightAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreatePointLightAction()
	{
		Description = "poly.CreatePointLight";
		ShortName = "Create Point Light";
	}
public:
	bool Execute_Implementation(bool bEmitRecord) override;
};
