// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectorBase.h"
#include "SelectorVisualiserBase.generated.h"

UCLASS()
class POLY_API ASelectorVisualiserBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectorVisualiserBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnActorSelected(ASelectorBase* InSelector, AActor* InActor);
	void OnActorSelected_Implementation(ASelectorBase* InSelector, AActor* InActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnActorDeselected(ASelectorBase* InSelector, AActor* InActor);
	void OnActorDeselected_Implementation(ASelectorBase* InSelector, AActor* InActor);

	/** Please add a function description */
	void OnSelectorDestroyed(AActor* DestroyedActor);

public:	

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<ASelectorBase> Selector = nullptr;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	uint8 StencilValue = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UMaterialInterface> OverlayMaterial = nullptr;

};
