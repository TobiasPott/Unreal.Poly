// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ToolInterface.generated.h"


/**
 *
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UToolInterface : public UInterface
{
	GENERATED_BODY()
};

class IToolInterface
{
	GENERATED_BODY()

public:
	// ToDo: @tpott: Consider adding 'ToolUpdate' to send intermediate updates from an active tool (e.g. move update)
	/** Add interface function declarations here */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	bool CanActivate();
	virtual bool CanActivate_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	bool Activate();
	virtual bool Activate_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	bool Deactivate();
	virtual bool Deactivate_Implementation();

};
