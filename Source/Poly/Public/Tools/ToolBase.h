// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tools/ToolInterface.h"
#include "ToolBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToolBaseEvent, UToolBase*, Tool);

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

public:

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Default")
	FToolBaseEvent Activated;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Default")
	FToolBaseEvent Deactivated;
};
