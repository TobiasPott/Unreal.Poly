// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectorVisualiserBase.generated.h"

class ASelectorBase;

UCLASS()
class POLY_API ASelectorVisualiserBase : public AActor
{
	friend class ASelectorBase;

	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComponent;

public:	
	// Sets default values for this actor's properties
	ASelectorVisualiserBase();

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	bool bIsInitialised = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<ASelectorBase> Selector = nullptr;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UMaterialInterface> OverlayMaterial = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Please add a function description */
	void Init();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnSelectableSelected(ASelectorBase* InSelector, USelectableBase* InSelectable);
	void OnSelectableSelected_Implementation(ASelectorBase* InSelector, USelectableBase* InSelectable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnSelectableDeselected(ASelectorBase* InSelector, USelectableBase* InSelectable);
	void OnSelectableDeselected_Implementation(ASelectorBase* InSelector, USelectableBase* InSelectable);
	
	UFUNCTION()
	void OnSelectorDestroyed(AActor* DestroyedActor);


};
