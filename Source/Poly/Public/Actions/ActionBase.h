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

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	uint8 Category = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int32 Target = -1;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	FString Description = "poly.Action";


public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual uint8 GetCategory() { return Category; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual int32 GetTarget() { return Target; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	virtual FString GetDescription() { return Description; };

	UFUNCTION(BlueprintCallable, Category = "Default")
	virtual UActionRef* GetUnique();


public:
	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	bool Execute(bool bSilent = false, bool bEmitRecord = true);
	virtual bool Execute_Implementation(bool bSilent = false, bool bEmitRecord = true) { return false; };

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