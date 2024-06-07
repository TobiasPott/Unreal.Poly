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
	// Ctors
	UActionBase()
	{
		Description = "poly.ActionBase";
		ShortName = "<ActionBase>";
	}
	UActionBase(const FString& InDescription, const FString& InShortName)
	{
		Description = InDescription;
		ShortName = InShortName;
	}

	UPROPERTY(VisibleAnywhere, Category = "Default")
	FString Description = "poly.Action";
	UPROPERTY(VisibleAnywhere, Category = "Default")
	FString ShortName = "Action";

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	uint8 Indent = 0; // only used for UI nesting and collection nesting tracking

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual FString GetDescription() { return this->Description; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual FString GetShortName() { return this->ShortName; };

	UFUNCTION(BlueprintCallable, Category = "Default")
	virtual void SetIndent(const uint8 InIndent) { this->Indent = InIndent; };

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

