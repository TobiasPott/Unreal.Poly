// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "SelectableBase.generated.h"


UCLASS(Blueprintable, ClassGroup = (Interaction, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class POLY_API USelectableBase : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = true;


	// Sets default values for this component's properties
	USelectableBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);
	virtual void OnActorClicked_Implementation(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void ChangeState(const bool bIsSelected);
	virtual void ChangeState_Implementation(const bool bIsSelected);
		
};
