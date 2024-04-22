// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoBase.h"
#include "TranslateGizmo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class POLY_API ATranslateGizmo : public AGizmoBase
{
	GENERATED_BODY()

public:

	ATranslateGizmo();

	virtual EGizmoType GetGizmoType() const final { return EGizmoType::TT_Translation; }

	virtual FTransform GetDeltaTransform(const FVector& LookingVector
		, const FVector& RayStartPoint
		, const FVector& RayEndPoint,  EGizmoDomain Domain) override;

	// Returns a Snapped Transform based on how much has been accumulated, the Delta Transform and Snapping Value
	virtual FTransform GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
		, const FTransform& DeltaTransform
		, EGizmoDomain Domain
		, float SnappingValue) const override;


protected:

	// The Hit Box for the XY-Plane Translation
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class UBoxComponent* XY_PlaneBox;

	// The Hit Box for the YZ-Plane Translation
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class UBoxComponent* YZ_PlaneBox;

	// The Hit Box for the	XZ-Plane Translation
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class UBoxComponent* XZ_PlaneBox;

	// The Hit Box for the	XYZ Free Translation
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class USphereComponent* XYZ_Sphere;
};

