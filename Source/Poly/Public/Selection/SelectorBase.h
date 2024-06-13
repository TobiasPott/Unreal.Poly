// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Functions/Poly_SelectorFunctions.h"
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
	UFUNCTION(BlueprintCallable, Category = "Selector")
	bool IsSelected(UPolySelection* InSelectable) { return  this->Selection.Contains(InSelectable); }

	UFUNCTION(BlueprintCallable, Category = "Selector")
	void Select(UPolySelection* InSelectable, bool& IsSelected) { SelectT(InSelectable, IsSelected); }
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void Deselect(UPolySelection* InSelectable, bool& IsSelected) { DeselectT(InSelectable, IsSelected); }
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void Replace(UPolySelection* InSelectable, bool& IsSelected) { ReplaceT(InSelectable, IsSelected); }


	UFUNCTION(BlueprintCallable, Category = "Selector")
	void SelectAll(const TArray<UPolySelection*>& InSelectables) { SelectAllT(InSelectables); }
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void DeselectAll(const TArray<UPolySelection*>& InSelectables) { DeselectAllT(InSelectables); }
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void ReplaceAll(const TArray<UPolySelection*>& InSelectables) { ReplaceAllT(InSelectables); }

	UFUNCTION(BlueprintCallable, Category = "Intern")
	void ClearSelection();

public:

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void SelectT(TPolySelectionType* InSelectable, bool& IsSelected)
	{
		if (!Selection.IsEmpty() && IsSingleSelection)
			this->ClearSelection();

		if (!IsValid(InSelectable))
		{
			IsSelected = false;
			return;
		}
		Selection.AddUnique(InSelectable);
		IsSelected = Selection.Contains(InSelectable);


		if (IsSelected)
			if (this->SelectableSelected.IsBound())
				this->SelectableSelected.Broadcast(this, InSelectable);
	}

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void DeselectT(TPolySelectionType* InSelectable, bool& IsSelected)
	{
		if (!Selection.Contains(InSelectable))
		{
			IsSelected = false;
			return;
		}

		Selection.Remove(InSelectable);
		IsSelected = Selection.Contains(InSelectable);

		if (!IsSelected)
			if (this->SelectableDeselected.IsBound())
				this->SelectableDeselected.Broadcast(this, InSelectable);

	}
	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void ReplaceT(TPolySelectionType* InSelectable, bool& IsSelected)
	{
		this->ClearSelection();
		this->SelectT(InSelectable, IsSelected);
	}
	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void SelectAllT(const TArray<TPolySelectionType*>& InSelectables)
	{
		bool IsSelected = false;
		for (int i = 0; i < InSelectables.Num(); i++)
			this->SelectT(InSelectables[i], IsSelected);
	}
	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void DeselectAllT(const TArray<TPolySelectionType*>& InSelectables)
	{
		bool IsSelected = false;
		for (int i = 0; i < InSelectables.Num(); i++)
			this->DeselectT(InSelectables[i], IsSelected);
	}
	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void ReplaceAllT(const TArray<TPolySelectionType*>& InSelectables)
	{
		this->ClearSelection();
		this->SelectAllT(InSelectables);
	}



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
