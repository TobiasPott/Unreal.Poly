// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "Actions/ActionBase.h"
#include "PolyMeshActor.h"
#include "PolyMesh_CreationFunctions.generated.h"

/**
 * 
 */
UCLASS()
class POLY_API UPolyMesh_CreationFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Poly|Create", meta = (WorldContext = "WorldContext"))
	static void CreateBoxActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset = FVector::ZeroVector, const FVector InDimensions = FVector(100, 100, 100));


};


/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UCreateBoxAction: public UActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCreateBoxAction();


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Actions|Creation")
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Actions|Creation")
	FVector Dimensions = FVector::ZeroVector;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};