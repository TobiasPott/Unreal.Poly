// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/UserWidgetTypes.h"
#include "PolyHUD.generated.h"

/**
 *
 */
UCLASS()
class POLY_API APolyHUD : public AHUD
{
	GENERATED_BODY()

public:
	APolyHUD();

};



UCLASS(Blueprintable)
class POLY_API UActorSelectionRequest : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	EActorSelectionRequestMode Mode = EActorSelectionRequestMode::Click;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	bool bSubmitted = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FVector2D FirstPoint = FVector2D(0, 0);
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FVector2D SecondPoint = FVector2D(0, 0);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	bool bIncludeNonCollider = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	bool bOnlyEnclosed = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	TSubclassOf<AActor> FilterClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	TArray<AActor*> Actors;


public:
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Init(const EActorSelectionRequestMode InMode, const FVector2D& InFirstPoint, const FVector2D& InSecondPoint, TSubclassOf<AActor> InFilterClass,
		bool bInIncludeNonCollider = false, bool bInOnlyEnclosed = false)
	{
		this->Mode = InMode;
		this->FirstPoint = InFirstPoint;
		this->SecondPoint = InSecondPoint;
		this->bIncludeNonCollider = bInIncludeNonCollider;
		this->bOnlyEnclosed = bInOnlyEnclosed;
		this->FilterClass = InFilterClass.Get();
	}


	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Submit() { this->bSubmitted = true; }
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void UpdateSecondPoint(const FVector2D InSecondPoint) { this->SecondPoint = InSecondPoint; }


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	int32 Count() { return Actors.Num(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	bool IsEmpty() { return Actors.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	bool IsNotEmpty() { return !Actors.IsEmpty(); }

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void OnFinished() 
	{
		if (Finished.IsBound())
			Finished.Broadcast(this, !Actors.IsEmpty());
	}


	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetActors(TArray<AActor*> InActors)
	{
		Actors.Reset(InActors.Num());
		for (int i = 0; i < InActors.Num(); i++)
			Actors.Add(InActors[i]);
	}


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMarqueeSelectionRequestFinished, UActorSelectionRequest*, Request, bool, bSuccess);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FMarqueeSelectionRequestFinished Finished;

};
