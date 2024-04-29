// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tools/ToolInterface.h"
#include "Tools/ToolBase.h"
#include "ToolBaseActor.generated.h"


/**
 *
 */
UCLASS()
class POLY_API AToolBaseActor : public AActor, public IToolInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Intern")
	UToolBase* Tool = nullptr;

protected:
	void OnToolActivated(UToolBase* InTool);
	void OnToolDeactivated(UToolBase* InTool);
	void OnToolActionEmitted(UToolBase* InTool, bool bEmitted, UActionBase* Action);

	bool Activate_Implementation() override;
	bool Deactivate_Implementation() override;


public:

	UFUNCTION(BlueprintCallable, Category = "Intern")
	void SetTool(UToolBase* NewTool);

public:

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Default")
	FToolBaseEvent ToolActivated;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Default")
	FToolBaseEvent ToolDeactivated;
};
