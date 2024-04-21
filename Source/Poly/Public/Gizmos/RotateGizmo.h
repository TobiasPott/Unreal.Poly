// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gizmos/GizmoBase.h"
#include "RotateGizmo.generated.h"

/**
 *
 */
UCLASS()
class POLY_API ARotateGizmo : public AGizmoBase
{
	GENERATED_BODY()

public:

	ARotateGizmo();

	virtual EGizmoType GetGizmoType() const final { return EGizmoType::TT_Rotation; }

	// Returns a Snapped Transform based on how much has been accumulated, the Delta Transform and Snapping Value
	virtual FTransform GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
		, const FTransform& DeltaTransform
		, EGizmoDomain Domain
		, float SnappingValue) const override;

protected:

	//Rotation has a special way of Handling the Scene Scaling and that is, that its AXis need to face the Camera as well!
	virtual FVector CalculateGizmoSceneScale(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection
		, float FieldOfView) override;

	virtual FTransform GetDeltaTransform(const FVector& LookingVector
		, const FVector& RayStartPoint
		, const FVector& RayEndPoint
		, EGizmoDomain Domain) override;

private:

	FVector PreviousRotationViewScale;
};
