// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ActionBase.h"
#include "ActionCollection.generated.h"

/**
 *
 */

UCLASS(Blueprintable)
class POLY_API UActionCollection : public UActionBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	bool bIsRoot = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TArray<UActionBase*> Actions;


public:

	UFUNCTION(BlueprintCallable, Category = "Default")
	void AddItems(TArray<UActionBase*> InActions);
	UFUNCTION(BlueprintCallable, Category = "Default")
	void AddItem(UActionBase* InAction);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	int32 NumOfActions() { return this->Actions.Num(); };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	int32 NumOfCollections() {
		return this->Actions.FilterByPredicate([](const UActionBase* Act) {
			return Act->IsA<UActionCollection>();
			}).Num();
	};
	UFUNCTION(BlueprintCallable, Category = "Default")
	TArray<UActionBase*>& GetActions() { return this->Actions; };


	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetShortNameAndDescription(const FString InShortName, const FString InDescription)
	{
		this->ShortName = InShortName;
		this->Description = InDescription;
	};

	virtual void SetIndent(const uint8 InIndent) override;

public:
	/** Please add a function description */
	virtual bool Execute_Implementation(bool bEmitRecord = true) override { return false; };

	virtual void Submit_Implementation() override { };

	virtual void Discard_Implementation() override { };


};