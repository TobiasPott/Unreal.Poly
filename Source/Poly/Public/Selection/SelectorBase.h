// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectableBase.h"
#include "SelectorVisualiserBase.h"
#include "SelectorBase.generated.h"


UCLASS()
class POLY_API ASelectorBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComponent;


public:	
	// Sets default values for this actor's properties
	ASelectorBase();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool IsSingleSelection = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	uint8 Stencil = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<USelectableBase*> Selection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UClass> VisualiserClass = ASelectorVisualiserBase::StaticClass();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() { return Selection.IsEmpty(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsSelected(USelectableBase* InSelectable);
	virtual bool IsSelected_Implementation(USelectableBase* InSelectable);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Select(USelectableBase* InSelectable, bool& IsSelected);
	void Select_Implementation(USelectableBase* InSelectable, bool& IsSelected);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Deselect(USelectableBase* InSelectable, bool& IsSelected);
	void Deselect_Implementation(USelectableBase* InSelectable, bool& IsSelected);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Replace(USelectableBase* InSelectable, bool& IsSelected);
	void Replace_Implementation(USelectableBase* InSelectable, bool& IsSelected);

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Intern")
	void ClearSelection();
	void ClearSelection_Implementation();


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectableSelected, ASelectorBase*, Selector, USelectableBase*, Selectable);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FSelectableSelected SelectableSelected;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FSelectableSelected SelectableDeselected;


};
