// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoTypes.h"
#include "Gizmos/GizmoCore.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Gizmos/SelectGizmo.h"
#include "Gizmos/ElementsGizmo.h"
#include "GizmoBaseActor_Intern.h"
#include "GizmoBaseActor.generated.h"

UCLASS()
class POLY_API AGizmoBaseActor : public AGizmoBaseActor_Intern
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGizmoBaseActor();


	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	EGizmoExtType Type = EGizmoExtType::GET_NoTransform;


	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Default")
	FGizmoPivot Pivot = FGizmoPivot();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	EGizmoPivotSource PivotLocationSource = EGizmoPivotSource::PS_Center;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	EGizmoPivotSource PivotOrientationSource = EGizmoPivotSource::PS_Identity;


	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	FName SelectorName = "Default";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UClass> SelectClass = AActor::StaticClass();


	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	bool bHasActorSelection = false;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	bool bHasElementSelection = false;


protected:
	/** Please add a function description */
	void CreateTranslateCore_Implementation(ATranslateGizmo*& OutTranslateCore) override;
	void CreateRotateCore_Implementation(ARotateGizmo*& OutRotateCore) override;
	void CreateScaleCore_Implementation(AScaleGizmo*& OutScaleCore) override;
	void CreateSelectCore_Implementation(ASelectGizmo*& OutSelectCore) override;
	void CreateElementsCore_Implementation(AElementsGizmo*& OutElementsCore) override;


	/** Please add a function description */
	void Translate_TranslationChanged_Implementation(bool bEnded, FVector DeltaTranslation) override;
	void Translate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform) override;

	/** Please add a function description */
	void Rotate_RotationChanged_Implementation(bool bEnded, FRotator DeltaRotation) override;
	void Rotate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform) override;

	/** Please add a function description */
	void Scale_ScaleChanged_Implementation(bool bEnded, FVector DeltaScale) override;
	void Scale_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform) override;

	/** Please add a function description */
	void Select_Finished_Implementation(UActorSelectionRequest* Request, bool bSuccess) override;
	void Elements_Finished_Implementation(AElementsGizmo* Core) override;



	UFUNCTION(BlueprintCallable, Category = "Intern")
	void SetupCores();


	UFUNCTION(BlueprintCallable, Category = "Intern")
	void TransformSelection(FTransform DeltaTransform, bool bInLocalSpace);
	UFUNCTION(BlueprintCallable, Category = "Intern")
	void TransformCore(FTransform DeltaTransform, bool bInLocalSpace, AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Intern")
	void UpdateGizmoSpace(ETransformSpace InSpace);


	UFUNCTION(BlueprintCallable, Category = "Intern")
	void UpdatePivot(bool bRefreshLocation = true, bool bRefreshOrientation = true);
	UFUNCTION(BlueprintCallable, Category = "Intern")
	FVector GetPivotLocationFromSelection();
	UFUNCTION(BlueprintCallable, Category = "Intern")
	FRotator GetPivotOrientationFromSelection();	
	
	UFUNCTION(BlueprintCallable, Category = "Intern")
	void SetPivotLocationSource(const EGizmoPivotSource InLocationSource) {
		this->PivotLocationSource = InLocationSource;
	};
	UFUNCTION(BlueprintCallable, Category = "Intern")
	void SetPivotOrientationSource(const EGizmoPivotSource InOrientationSource) {
		this->PivotOrientationSource = InOrientationSource;
	};


public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

};
