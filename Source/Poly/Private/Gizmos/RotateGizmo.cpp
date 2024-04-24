// Copyright 2020 Juan Marcelo Portillo. All Rights Reserved.

#include "Gizmos/RotateGizmo.h"

ARotateGizmo::ARotateGizmo()
{
	// set higher arc radius to scale rotate gizmo slightly smaller than scale and translate
	CameraArcRadius = 150.0f;
}

FTransform ARotateGizmo::GetDeltaTransform(const FVector& LookingVector, const FVector& RayStartPoint
	, const FVector& RayEndPoint, EGizmoDomain Domain)
{
	FTransform deltaTransform;
	deltaTransform.SetScale3D(FVector::ZeroVector);

	if (AreRaysValid())
	{
		FVector planeNormal = FVector(1.f, 0.f, 0.f);

		switch (Domain)
		{
		case EGizmoDomain::TD_X_Axis:planeNormal = GetActorForwardVector(); break;
		case EGizmoDomain::TD_Y_Axis: planeNormal = GetActorRightVector(); break;
		case EGizmoDomain::TD_Z_Axis: planeNormal = GetActorUpVector(); break;
		}

		const FVector gizmoLocation = GetActorLocation();

		FPlane plane;
		plane.X = planeNormal.X;
		plane.Y = planeNormal.Y;
		plane.Z = planeNormal.Z;
		plane.W = FVector::PointPlaneDist(gizmoLocation, FVector::ZeroVector, planeNormal);

		FVector deltaLocation = FMath::LinePlaneIntersection(RayStartPoint, RayEndPoint, plane) - gizmoLocation;
		FVector prevDeltaLocation = FMath::LinePlaneIntersection(PreviousRayStartPoint, PreviousRayEndPoint, plane) - gizmoLocation;

		//determining direction of Angle
		float factor = (FVector::DotProduct(FVector::CrossProduct(deltaLocation, prevDeltaLocation), planeNormal)) >= 0.f ?
			-1.f : 1.f;

		FVector diffOfDeltas = deltaLocation - prevDeltaLocation;

		deltaLocation.Normalize();
		prevDeltaLocation.Normalize();

		float angle = diffOfDeltas.Size() < 0.01f ? 0.f : FMath::Acos(FVector::DotProduct(deltaLocation, prevDeltaLocation));

		angle *= factor;
		FQuat rotQuat = FQuat(planeNormal, angle);
		deltaTransform.SetRotation(rotQuat);

		// Call 'Changed' events
		if (!deltaTransform.RotationEquals(FTransform::Identity, 0.000001))
		{
			if (TransformChanged.IsBound())
				TransformChanged.Broadcast(false, deltaTransform);
			if (RotationChanged.IsBound())
				RotationChanged.Broadcast(false, deltaTransform.Rotator());
		}
	}

	UpdateRays(RayStartPoint, RayEndPoint);

	return deltaTransform;
}

FTransform ARotateGizmo::GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
	, const FTransform& DeltaTransform
	, EGizmoDomain Domain
	, float SnappingValue) const
{
	if (SnappingValue == 0.f) return DeltaTransform;

	FTransform result = DeltaTransform;

	FRotator addedRotation = outCurrentAccumulatedTransform.GetRotation().Rotator()
		+ DeltaTransform.GetRotation().Rotator();

	FRotator snappedRotation = addedRotation.GridSnap(FRotator(SnappingValue));
	result.SetRotation(snappedRotation.Quaternion());
	outCurrentAccumulatedTransform.SetRotation((addedRotation - snappedRotation).Quaternion());

	return result;
}
