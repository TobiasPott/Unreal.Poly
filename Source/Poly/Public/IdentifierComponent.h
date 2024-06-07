// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IdentifierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POLY_API UIdentifierComponent : public UActorComponent
{
	friend class UPoly_IdentifierFunctions;

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIdentifierComponent();

private:
	static int32 NextId;
	static TArray<UIdentifierComponent*> AllComponents;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	int32 Id = -1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	FString DisplayName = FString("Unnamed");

	// ToDo: PONDER: @tpott: Ponder about adding a flag or sort of for auto-generated Ids 
	//				Intent is to indicate which Identifiers where created upon query for one (and have not been initialised before)


	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;



	UFUNCTION(BlueprintCallable, Category = "Identifier")
	void SetAutoIdentifier(const FString& NewDisplayName);
	UFUNCTION(BlueprintCallable, Category = "Identifier")
	void SetIdentifier(const int32& NewId, const FString& NewDisplayName);


	UFUNCTION()
	static int32 GetNextId();

};
