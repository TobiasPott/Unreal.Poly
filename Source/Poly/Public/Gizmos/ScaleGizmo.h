// Copyright 2020 Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoCoreActor.h"
#include "Gizmos/TranslateGizmo.h"
#include "ScaleGizmo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class POLY_API AScaleGizmo : public ATranslateGizmo
{
	GENERATED_BODY()

public:

	AScaleGizmo();

	virtual EGizmoType GetGizmoType() const final { return EGizmoType::GT_Scale; }

	virtual void UpdateGizmoSpace(ETransformSpace SpaceType);

	virtual FTransform GetDeltaTransform(const FVector& LookingVector, const FVector& RayStartPoint, const FVector& RayEndPoint, EGizmoDomain Domain, bool bSilent = false) override;

	//// Returns a Snapped Transform based on how much has been accumulated, the Delta Transform and Snapping Value
	//virtual FTransform GetSnappedTransform(FTransform& outCurrentAccumulatedTransform, const FTransform& DeltaTransform, EGizmoDomain Domain, float SnappingValue) const override;

	//virtual FTransform GetSnappedTransformPerComponent(const FTransform& OldComponentTransform, const FTransform& NewComponentTransform, EGizmoDomain Domain, float SnappingValue) const override;

protected:
	//To see how much an Unreal Unit affects Scaling (e.g. how powerful the mouse scales the object!)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo")
	float ScalingFactor = 0.1;

};
