// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectorBase.generated.h"

//class USelectableBase;

UCLASS()
class POLY_API ASelectorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectorBase();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<AActor*> Selection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsSelected(AActor* InActor);
	virtual bool IsSelected_Implementation(AActor* InActor);

};
