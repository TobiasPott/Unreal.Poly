// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectorTypes.h"
#include "SelectableBase.generated.h"


UCLASS(Blueprintable, ClassGroup = (Interaction, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class POLY_API USelectableBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USelectableBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selectable")
	bool bEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selectable")
	FName SelectorName = USelectorNames::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selectable")
	ESelectorChannel Channel = ESelectorChannel::Default;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);
	virtual void OnActorClicked_Implementation(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	void ChangeState(const bool bIsSelected);
	virtual void ChangeState_Implementation(const bool bIsSelected);

	// DeterminesOutputType = "InClass", 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	bool GetSelector(ASelectorBase*& OutActor);
	virtual bool GetSelector_Implementation(ASelectorBase*& OutActor);


	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	void SetMaterialForState(bool IsSelected, UMaterialInterface* SelectedMaterial);
	void SetMaterialForState_Implementation(bool IsSelected, UMaterialInterface* SelectedMaterial);

};
