// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/HUD.h"
#include "GenericPlatform/GenericApplication.h"
#include "Misc/App.h"
#include "EngineGlobals.h"
#include "Layout/Margin.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "EngineUtils.h"
#include "Engine/Canvas.h"
#include "Misc/UObjectToken.h"
#include "UObject/UObjectIterator.h"
#include "UI/UserWidgetTypes.h"
#include "ActorSelectionRequest.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorSelectionRequestFinished, class UActorSelectionRequest*, Request, bool, bSuccess);

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


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Trace")
	TEnumAsByte<ETraceTypeQuery> TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Trace")
	bool bTraceComplex = true;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Trace")
	float TraceDistance = MAX_FLT;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	TSubclassOf<AActor> FilterClass;

	UPROPERTY(VisibleAnywhere, Category = "Selection")
	TArray<AActor*> Actors;


public:
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Init(const EActorSelectionRequestMode InMode, const FVector2D& InFirstPoint, const FVector2D& InSecondPoint, TSubclassOf<AActor> InFilterClass, bool bInIncludeNonCollider = false, bool bInOnlyEnclosed = false);


	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Submit() { this->bSubmitted = true; }
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void UpdateSecondPoint(const FVector2D InSecondPoint) { this->SecondPoint = InSecondPoint; }
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetTrace(const TEnumAsByte<ETraceTypeQuery> InTraceChannel, bool bInTraceComplex = true, float InTraceDistance = 100000000)
	{
		TraceChannel = InTraceChannel;
		bTraceComplex = bInTraceComplex;
		TraceDistance = TraceDistance;
	}



	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	int32 Count() { return Actors.Num(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	bool IsEmpty() { return Actors.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	bool IsNotEmpty() { return !Actors.IsEmpty(); }

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void OnFinished();


	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetActors(TArray<AActor*> InActors);
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Selection")
	TArray<AActor*>& GetActors() { return this->Actors; };


public:

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActorSelectionRequestFinished Finished;

};
