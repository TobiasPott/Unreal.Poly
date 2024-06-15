// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/ActionBase.h"
#include "ActionRunner.generated.h"

UCLASS()
class POLY_API AActionRunner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionRunner();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TArray<UActionBase*> Actions;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	UActionBase* PendingAction = nullptr;

protected:
	UFUNCTION()
	void OnActionSubmitted(UActionBase* Action);
	UFUNCTION()
	void OnActionDiscarded(UActionBase* Action);

public:

	UFUNCTION()
	bool HasPennding() { return IsValid(PendingAction); };
	UFUNCTION()
	int32 NumOfActions() { return Actions.Num(); };


	UFUNCTION(BlueprintCallable, Category = "Default")
	bool Run(UActionBase* Action);



public:
	UFUNCTION(BlueprintCallable, Category = "Actions", meta = (WorldContext = "WorldContext"))
	static bool RunOnAny(const UObject* WorldContext,UActionBase* InAction);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionEvent, UActionBase*, Action);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActionEvent SubmittedAction;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActionEvent DiscardedAction;
};
