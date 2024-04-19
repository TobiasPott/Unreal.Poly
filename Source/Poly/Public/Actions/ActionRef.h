// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ActionBase.h"
#include "ActionRef.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UActionRef : public UActionBase
{
	friend class UActionBase;

	GENERATED_BODY()

private:
	static int32 InstanceCount;

	void Init(UActionBase* InAction);

public:
	// Sets default values for this actor's properties
	UActionRef();

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	int32 Id;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	UActionBase* Action = nullptr;

public:
	uint8 GetCategory() override { return Action->GetCategory(); };

	int32 GetTarget() override { return Action->GetTarget(); };

	FString GetDescription() override { return Action->GetDescription(); };

	FString GetShortName() override { return Action->GetShortName(); };

	UActionRef* GetUnique() override;

	bool Execute_Implementation(bool bEmitRecord) override;

};
