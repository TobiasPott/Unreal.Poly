// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Modeling/PolySelection.h"
#include "SelectorVisualiserBase.h"
#include "SelectorBase.generated.h"


UCLASS()
class POLY_API ASelectorBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;


public:	
	// Sets default values for this actor's properties
	ASelectorBase();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selector")
	bool IsSingleSelection = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selector")
	uint8 Stencil = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Selector")
	TArray<AActor*> SelectionOLD;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Selector")
	TArray<UPolySelection*> Selection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selector")
	TObjectPtr<UClass> VisualiserClass = ASelectorVisualiserBase::StaticClass();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() { return Selection.IsEmpty(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	bool IsSelected(UPolySelection* InSelectable);
	virtual bool IsSelected_Implementation(UPolySelection* InSelectable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	void Select(UPolySelection* InSelectable, bool& IsSelected);
	void Select_Implementation(UPolySelection* InSelectable, bool& IsSelected);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	void Deselect(UPolySelection* InSelectable, bool& IsSelected);
	void Deselect_Implementation(UPolySelection* InSelectable, bool& IsSelected);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	void Replace(UPolySelection* InSelectable, bool& IsSelected);
	void Replace_Implementation(UPolySelection* InSelectable, bool& IsSelected);


	UFUNCTION(BlueprintCallable, Category = "Selector")
	void SelectAll(const TArray<UPolySelection*>& InSelectables);
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void DeselectAll(const TArray<UPolySelection*>& InSelectables);
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void ReplaceAll(const TArray<UPolySelection*>& InSelectables);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Intern")
	void ClearSelection();
	void ClearSelection_Implementation();

protected:
	UFUNCTION()
	void SetVisualiser(TSubclassOf<ASelectorVisualiserBase> NewVisualiserClass);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectableSelected, ASelectorBase*, Selector, UPolySelection*, Selectable);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FSelectableSelected SelectableSelected;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FSelectableSelected SelectableDeselected;


};
