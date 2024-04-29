// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tools/ToolBase.h"
#include "Toolbox.generated.h"


UCLASS()
class POLY_API AToolbox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToolbox();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TScriptInterface<IToolInterface> ActiveTool = nullptr;

public:	

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool SetActive(TScriptInterface<IToolInterface> InTool);

};
