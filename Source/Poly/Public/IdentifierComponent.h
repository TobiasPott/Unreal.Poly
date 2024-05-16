// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IdentifierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POLY_API UIdentifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIdentifierComponent();

private:
	static int32 NextId;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	int32 Id = -1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FString DisplayName = FString("None");


	UFUNCTION(BlueprintCallable, Category = "Identifier")
	void SetAutoIdentifier(const FString& NewDisplayName);


	UFUNCTION(BlueprintCallable, Category = "Identifier")
	void SetIdentifier(const int32& NewId, const FString& NewDisplayName);


	UFUNCTION(BlueprintCallable, Category = "Identifier")
	static int32 GetNextId();

};
