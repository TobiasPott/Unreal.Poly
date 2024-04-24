// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/ActorSelectionRequest.h"
#include "Gizmos/GizmoCore.h"
#include "SelectGizmo.generated.h"

UCLASS(Blueprintable, BlueprintType)
class POLY_API ASelectGizmo : public AGizmoCore
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASelectGizmo();

protected:
	bool bIsMousePressed = false;
	bool bIsEnabled = false;

	UPROPERTY()
	int32 PlayerIndex = 0;
	UPROPERTY()
	FName InputAction = EKeys::LeftMouseButton.GetFName();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Filter")
	bool bDisableOnFinish = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FVector2D FirstPoint;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FVector2D SecondPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FHitResult HitResult;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	UActorSelectionRequest* Request = nullptr;


	// Filter values
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Filter")
	EActorSelectionRequestMode MarqueeMode = EActorSelectionRequestMode::Click;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Filter")
	TSubclassOf<AActor> FilterClass;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Filter")
	bool bIncludeNonCollider = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Filter")
	bool bIncludeOnlyEnclosed = false;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Setup(EActorSelectionRequestMode InMarqueeMode, UClass* InFilterClass, bool bInIncludeNonCollider = false, bool bInIncludeOnlyEnclosed = false, 
		bool bInDisableOnFinish = false);

	void SetEnabled(const bool bInEnable);

	virtual void SetGizmoHidden(const bool bHiddenInGame = false) override;

	UFUNCTION()
	virtual void OnInputKey_Pressed(FKey InKey);
	UFUNCTION()
	virtual void OnInputKey_Released(FKey InKey);
	UFUNCTION()
	virtual void OnMouseX(float AxisValue);
	UFUNCTION()
	virtual void OnMouseY(float AxisValue);
	UFUNCTION()
	virtual void OnMouse2D(FVector AxisValue);



	UFUNCTION(BlueprintCallable, Category = "Selection")
	virtual void OnRequestFinished(UActorSelectionRequest* InRequest, bool bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Selection")
	virtual void OnFinished();

public:

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FActorSelectionRequestFinished Finished;


};
