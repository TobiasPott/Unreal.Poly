// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoTypes.h"
#include "GizmoBase.generated.h"

UCLASS()
class POLY_API AGizmoBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGizmoBase();



	void OnTransformChanged(const bool bEndTransform, const FTransform InDelta);
	void OnTransformEnded(const FTransform InDelta);



	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	void SetGizmoHidden(const bool bHiddenInGame = false);


public:

	// Transform Changed & Ended events
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoDeltaTransformDelegate TransformChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoDeltaTransformDelegate TransformEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoTranslateTransformDelegate TranslationChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoTranslateTransformDelegate TranslationEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoRotateTransformDelegate RotationChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoRotateTransformDelegate RotationEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoScaleTransformDelegate ScaleChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoScaleTransformDelegate ScaleEnded;

};
