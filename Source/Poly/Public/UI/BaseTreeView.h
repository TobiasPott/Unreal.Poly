// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TreeView.h"
#include "BaseTreeView.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class POLY_API UBaseTreeView : public UTreeView
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ListView)
	bool bCollapseItemsOnClick = false;


	virtual void OnItemClickedInternal(UObject* ListItem) override;

	UFUNCTION(BlueprintCallable, Category = TreeView)
	bool GetItemExpansion(UObject* Item);
	UFUNCTION(BlueprintCallable, Category = TreeView)
	void ToggleItemExpansion(UObject* Item);

};
