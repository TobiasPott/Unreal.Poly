// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "UI/SelectionRequestBase.h"
#include "Gizmos/GizmoCore.h"
#include "ElementsGizmo.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElementsSelectionFinished, class AElementsGizmo*, Core);


UCLASS(Blueprintable, BlueprintType)
class POLY_API AElementsGizmo : public AGizmoCore
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElementsGizmo();

protected:
	bool bIsMousePressed = false;
	bool bIsEnabled = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Default")
	int32 PlayerIndex = 0;
	UPROPERTY()
	FName InputAction = EKeys::LeftMouseButton.GetFName();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Selection")
	bool bDisableOnFinish = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FVector2D FirstPoint;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	FVector2D SecondPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Selection")
	USelectionRequestBase* Request = nullptr;


	// Filter values
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Filter")
	EActorSelectionRequestMode MarqueeMode = EActorSelectionRequestMode::Click;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Filter")
	bool bIncludeOnlyEnclosed = false;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Selection")
	TMap<AActor*, FGeometryScriptMeshSelection> Selection;



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Setup(EActorSelectionRequestMode InMarqueeMode, bool bInIncludeOnlyEnclosed = false, bool bInDisableOnFinish = false);

	void SetEnabled(const bool bInEnable);

	virtual void SetGizmoHidden(const bool bHiddenInGame = false) override;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetTargets(const TArray<AActor*>& Targets);

protected:

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
	virtual void OnFinished();

	UFUNCTION(BlueprintCallable, Category = "Selection")
	virtual void Clear();


public:

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FElementsSelectionFinished Finished;


};
