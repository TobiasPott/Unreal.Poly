// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "IdentifierComponent.h"
#include "PolyMeshActor.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class POLY_API APolyMeshActor : public ADynamicMeshActor
{
	GENERATED_BODY()

public:
	APolyMeshActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UIdentifierComponent* IdentifierComponent;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FName Name = FName("PolyMeshActor");

	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetCollision(bool bIsEnabled);

};
