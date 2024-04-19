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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* SceneComponent;


public:	
	// Sets default values for this actor's properties
	ASelectorBase();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selector")
	bool IsSingleSelection = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selector")
	uint8 Stencil = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Selector")
	TArray<AActor*> Selection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selector")
	TObjectPtr<UClass> VisualiserClass = ASelectorVisualiserBase::StaticClass();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() { return Selection.IsEmpty(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	bool IsSelected(AActor* InSelectable);
	virtual bool IsSelected_Implementation(AActor* InSelectable);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	void Select(AActor* InSelectable, bool& IsSelected);
	void Select_Implementation(AActor* InSelectable, bool& IsSelected);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	void Deselect(AActor* InSelectable, bool& IsSelected);
	void Deselect_Implementation(AActor* InSelectable, bool& IsSelected);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selector")
	void Replace(AActor* InSelectable, bool& IsSelected);
	void Replace_Implementation(AActor* InSelectable, bool& IsSelected);


	UFUNCTION(BlueprintCallable, Category = "Selector")
	void SelectAll(const TArray<AActor*>& InSelectables);
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void DeselectAll(const TArray<AActor*>& InSelectables);
	UFUNCTION(BlueprintCallable, Category = "Selector")
	void ReplaceAll(const TArray<AActor*>& InSelectables);


protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Intern")
	void ClearSelection();
	void ClearSelection_Implementation();
	
	UFUNCTION()
	void SetVisualiser(TSubclassOf<ASelectorVisualiserBase> NewVisualiserClass);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectableSelected, ASelectorBase*, Selector, AActor*, Selectable);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FSelectableSelected SelectableSelected;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FSelectableSelected SelectableDeselected;


};
