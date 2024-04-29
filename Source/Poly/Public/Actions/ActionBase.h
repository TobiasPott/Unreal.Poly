// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionBase.generated.h"

class UActionRef;
/**
 *
 */

UCLASS(Blueprintable)
class POLY_API UActionBase : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Default")
	FString Description = "poly.Action";
	UPROPERTY(VisibleAnywhere, Category = "Default")
	FString ShortName = "Action";


public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual FString GetDescription() { return Description; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual FString GetShortName() { return ShortName; };

	UFUNCTION(BlueprintCallable, Category = "Default")
	virtual UActionRef* GetUnique();


public:
	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	bool Execute(bool bEmitRecord = true);
	virtual bool Execute_Implementation(bool bEmitRecord = true) { return false; };

	UFUNCTION(BlueprintNativeEvent, Category = "Default")
	void Submit();
	virtual void Submit_Implementation() { if (Submitted.IsBound()) Submitted.Broadcast(this); };

	UFUNCTION(BlueprintNativeEvent, Category = "Default")
	void Discard();
	virtual void Discard_Implementation() { if (Discarded.IsBound()) Discarded.Broadcast(this); };


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionEvent, UActionBase*, Action);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActionEvent Submitted;

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorSelected, ASelectorBase*, Selector, AActor*, Actor);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActionEvent Discarded;
};


UCLASS(Blueprintable)
class POLY_API UActionCollection : public UActionBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Default")
	TArray<UActionBase*> Actions;


public:

	UFUNCTION(BlueprintCallable, Category = "Default")
	void AddItems(TArray<UActionBase*> InActions);
	UFUNCTION(BlueprintCallable, Category = "Default")
	void AddItem(UActionBase* InAction);

	UFUNCTION(BlueprintCallable, Category = "Default")
	TArray<UActionBase*>& GetActions() { return this->Actions; };


	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetShortName(const FString InShortName) { this->ShortName = InShortName; };
	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetDescription(const FString InDescription) { this->Description = InDescription; };


	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetShortNameAndDescription(const FString InShortName, const FString InDescription) 
	{
		this->ShortName = InShortName;
		this->Description = InDescription;
	};


public:
	/** Please add a function description */
	virtual bool Execute_Implementation(bool bEmitRecord = true) override { return false; };

	virtual void Submit_Implementation() override { };

	virtual void Discard_Implementation() override { };


};