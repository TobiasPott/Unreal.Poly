// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ActionBase.h"
#include "Tools/ToolInterface.h"
#include "ToolBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToolBaseEvent, UToolBase*, Tool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FToolActionEmittedEvent, UToolBase*, Tool, bool, bEmitted, UActionBase*, EmittedAction);

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UToolBase : public UObject, public IToolInterface
{
	GENERATED_BODY()

protected: 
	void OnActivated();
	void OnDeactivated();


	bool Activate_Implementation() override;
	bool Deactivate_Implementation() override;


	UFUNCTION(BlueprintCallable, Category = "Default")
	bool EmitAction(bool bEmit, UActionBase* EmitAction);

public:

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Tool")
	FToolBaseEvent Activated;
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Tool")
	FToolBaseEvent Deactivated;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Tool")
	FToolActionEmittedEvent ActionEmitted;
};
