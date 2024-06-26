// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "Selection/SelectionRequestBase.h"
#include "Modeling/PolyMeshSelection.h"
#include "Gizmos/GizmoCore.h"
#include "ElementsGizmo.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElementsSelectionFinished, class AElementsGizmo*, Core);



UCLASS(Blueprintable, BlueprintType)
class POLY_API AElementsGizmo : public AGizmoCore
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDynamicMeshComponent* DynamicMeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDynamicMeshComponent* SelectionDynamicMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;

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
	ESelectionRequestMode MarqueeMode = ESelectionRequestMode::Click;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Filter")
	EPolySelectionMode SelectionMode = EPolySelectionMode::Replace;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Filter")
	EGeometryScriptMeshSelectionType SelectionType = EGeometryScriptMeshSelectionType::Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Filter")
	float Distance = 10000.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Filter")
	bool bInvert = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Filter")
	float WindingThreshold = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Filter")
	int MinTrianglePoints = 3;


protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Selection")
	TArray<UPolyMeshSelection*> PolySelection;

	UPROPERTY()
	TObjectPtr<UDynamicMeshPool> Pool;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void Setup(ESelectionRequestMode InMarqueeMode, const float InDistance = 10000.0, const EGeometryScriptMeshSelectionType InSelectionType = EGeometryScriptMeshSelectionType::Triangles, const bool bInDisableOnFinish = false);

	virtual void SetEnableConsumeInput(const bool bInEnable) override;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetTargets(const TArray<AActor*>& Targets);
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void ClearTargets();
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void ClearSelections();
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetSelectionMode(EPolySelectionMode InSelectionMode = EPolySelectionMode::Replace);
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetSelectionType(EGeometryScriptMeshSelectionType InSelectionType = EGeometryScriptMeshSelectionType::Triangles);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Intern")
	void UpdateSelectionMesh(const FVector2D FirstScreenPoint, const FVector2D SecondScreenPoint);

	void UpdateSelection();
	void UpdateSelectionVisuals();


	bool IsEmptySelection() const;
	bool IsEmpty() { return this->PolySelection.IsEmpty(); };
	bool IsNotEmpty() { return !this->PolySelection.IsEmpty(); };
	TArray<UPolyMeshSelection*> GetPolySelection() { return this->PolySelection; };

	UPolyMeshSelection* GetFirstSelected() { return this->PolySelection.Num() > 0 ? this->PolySelection[0] : nullptr; };
	UPolyMeshSelection* GetLastSelected() { return this->PolySelection.Num() > 0 ? this->PolySelection[this->PolySelection.Num() - 1] : nullptr; };

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
