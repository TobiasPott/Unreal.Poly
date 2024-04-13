// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectableBase.h"
#include "SelectorBase.generated.h"

//class USelectableBase;

UCLASS()
class POLY_API ASelectorBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASelectorBase();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<AActor*> Selection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool IsSingleSelection = true;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsSelected(AActor* InActor);
	virtual bool IsSelected_Implementation(AActor* InActor);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Select(AActor* InActor, bool& IsSelected);
	void Select_Implementation(AActor* InActor, bool& IsSelected);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Deselect(AActor* InActor, bool& IsSelected);
	void Deselect_Implementation(AActor* InActor, bool& IsSelected);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Replace(AActor* InActor, bool& IsSelected);
	void Replace_Implementation(AActor* InActor, bool& IsSelected);

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Intern")
	void ClearSelection();
	void ClearSelection_Implementation();


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorSelected, ASelectorBase*, Selector, AActor*, Actor);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActorSelected ActorSelected;

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorSelected, ASelectorBase*, Selector, AActor*, Actor);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActorSelected ActorDeselected;


};
