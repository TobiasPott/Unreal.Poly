// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoBaseActor.h"
#include "RotateGizmo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class POLY_API ARotateGizmo : public AGizmoBaseActor
{
	GENERATED_BODY()

public:

	ARotateGizmo();

	virtual EGizmoType GetGizmoType() const final { return EGizmoType::GT_Rotation; }

	// Returns a Snapped Transform based on how much has been accumulated, the Delta Transform and Snapping Value
	virtual FTransform GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
		, const FTransform& DeltaTransform
		, EGizmoDomain Domain
		, float SnappingValue) const override;
	
protected:

	virtual FTransform GetDeltaTransform(const FVector& LookingVector
		, const FVector& RayStartPoint
		, const FVector& RayEndPoint
		, EGizmoDomain Domain) override;

private:

};
