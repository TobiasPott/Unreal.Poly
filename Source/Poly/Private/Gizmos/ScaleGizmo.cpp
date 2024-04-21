// Copyright 2020 Juan Marcelo Portillo. All Rights Reserved.


#include "Gizmos/ScaleGizmo.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AScaleGizmo::AScaleGizmo()
{
	XY_PlaneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("XY Plane"));
	XY_PlaneBox->SetComponentTickEnabled(false);
	YZ_PlaneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("YZ Plane"));
	YZ_PlaneBox->SetComponentTickEnabled(false);
	XZ_PlaneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("XZ Plane"));
	XZ_PlaneBox->SetComponentTickEnabled(false);
	XYZ_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("XYZ Sphere"));
	XYZ_Sphere->SetComponentTickEnabled(false);

	XY_PlaneBox->SetupAttachment(ScalingScene);
	YZ_PlaneBox->SetupAttachment(ScalingScene);
	XZ_PlaneBox->SetupAttachment(ScalingScene);
	XYZ_Sphere->SetupAttachment(ScalingScene);

	RegisterDomainComponent(XY_PlaneBox, EGizmoDomain::TD_XY_Plane);
	RegisterDomainComponent(YZ_PlaneBox, EGizmoDomain::TD_YZ_Plane);
	RegisterDomainComponent(XZ_PlaneBox, EGizmoDomain::TD_XZ_Plane);
	RegisterDomainComponent(XYZ_Sphere, EGizmoDomain::TD_XYZ);

	ScalingFactor = 0.05f;

}

void AScaleGizmo::UpdateGizmoSpace(ETransformSpace SpaceType)
{
	//Force to always be Local
	SetActorRelativeRotation(FQuat(EForceInit::ForceInit));
}

FTransform AScaleGizmo::GetDeltaTransform(const FVector& LookingVector
	, const FVector& RayStartPoint, const FVector& RayEndPoint
	, EGizmoDomain Domain)
{
	FTransform deltaTransform;
	deltaTransform.SetScale3D(FVector::ZeroVector);

	if (AreRaysValid())
	{
		const float Cos45Deg = 0.707;

		// the opposite direction of the Normal that is most perpendicular to the Looking Vector
		// will be the one we choose to be the normal to the Domain! (needs to be calculated for axis. For planes, it's straightforward)
		FVector planeNormal;

		FVector targetDirection(0.f);

		//we have two vectors for each direction, one world and one local
		// world is to calculate the scale direction (e.g. for X scale it would have to be multiplied by 1.f, 0.f, 0.f)
		// local is to calculate the direction of the mouse and calculate how much it moved!

		FVector forwardVector = GetActorForwardVector();
		FVector rightVector = GetActorRightVector();
		FVector upVector = GetActorUpVector();;


		switch (Domain)
		{
		case EGizmoDomain::TD_X_Axis:
		{
			targetDirection = forwardVector;
			if (FMath::Abs(FVector::DotProduct(LookingVector, rightVector)) > Cos45Deg) 
				planeNormal = rightVector;
			else 
				planeNormal = upVector;
			break;
		}
		case EGizmoDomain::TD_Y_Axis:
		{
			targetDirection = rightVector;
			if (FMath::Abs(FVector::DotProduct(LookingVector, forwardVector)) > Cos45Deg) 
				planeNormal = forwardVector;
			else 
				planeNormal = upVector;
			break;
		}
		case EGizmoDomain::TD_Z_Axis:
		{
			targetDirection = upVector;
			if (FMath::Abs(FVector::DotProduct(LookingVector, forwardVector)) > Cos45Deg) 
				planeNormal = forwardVector;
			else 
				planeNormal = rightVector;
			break;
		}
		case EGizmoDomain::TD_XY_Plane:
			targetDirection = forwardVector + rightVector;
			planeNormal = upVector;
			break;
		case EGizmoDomain::TD_YZ_Plane:
			targetDirection = rightVector + upVector;
			planeNormal = forwardVector;
			break;
		case EGizmoDomain::TD_XZ_Plane:
			targetDirection = forwardVector + upVector;
			planeNormal = rightVector;
			break;
		case EGizmoDomain::TD_XYZ:
			targetDirection = forwardVector + rightVector + upVector;
			planeNormal = LookingVector;
			break;
		}

		FPlane plane;
		plane.X = planeNormal.X;
		plane.Y = planeNormal.Y;
		plane.Z = planeNormal.Z;
		plane.W = FVector::PointPlaneDist(GetActorLocation(), FVector::ZeroVector, planeNormal);

		FVector deltaLocation =
			FMath::LinePlaneIntersection(RayStartPoint, RayEndPoint, plane)
			- FMath::LinePlaneIntersection(PreviousRayStartPoint, PreviousRayEndPoint, plane);

		deltaLocation = deltaLocation.ProjectOnTo(targetDirection);
		deltaTransform.SetScale3D(deltaLocation * ScalingFactor);

		// Call 'Changed' events
		if (!deltaTransform.Scale3DEquals(FTransform::Identity, 0.000001))
		{
			if (TransformChanged.IsBound())
				TransformChanged.Broadcast(false, deltaTransform);
			if (ScaleChanged.IsBound())
				ScaleChanged.Broadcast(false, deltaTransform.GetScale3D());
		}
	}

	UpdateRays(RayStartPoint, RayEndPoint);

	return deltaTransform;
}

FTransform AScaleGizmo::GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
	, const FTransform& DeltaTransform
	, EGizmoDomain Domain
	, float SnappingValue) const
{
	if (SnappingValue == 0.f) return DeltaTransform;

	FTransform result = DeltaTransform;
	FVector addedScale = outCurrentAccumulatedTransform.GetScale3D() + DeltaTransform.GetScale3D();

	float domains = 1.f;

	switch (Domain)
	{
	case EGizmoDomain::TD_XY_Plane:
	case EGizmoDomain::TD_YZ_Plane:
	case EGizmoDomain::TD_XZ_Plane:
		domains = 2.f;
		break;
	case EGizmoDomain::TD_XYZ:
		domains = 3.f;
		break;
	}

	FVector snappedScale = addedScale.GetSafeNormal()
		* FMath::GridSnap(addedScale.Size(), FMath::Sqrt(FMath::Square(SnappingValue) * domains));

	result.SetScale3D(snappedScale);
	outCurrentAccumulatedTransform.SetScale3D(addedScale - snappedScale);
	return result;
}

FTransform AScaleGizmo::GetSnappedTransformPerComponent(const FTransform& OldComponentTransform
	, const FTransform& NewComponentTransform, EGizmoDomain Domain
	, float SnappingValue) const
{

	FTransform result= NewComponentTransform;

	FVector newScale = NewComponentTransform.GetScale3D();
	if (!newScale.Equals(OldComponentTransform.GetScale3D(), 0.0001f))
	{
		FVector domainScale;
		switch (Domain)
		{
		case EGizmoDomain::TD_X_Axis: domainScale = FVector(1.f, 0.f, 0.f); break;
		case EGizmoDomain::TD_Y_Axis: domainScale = FVector(0.f, 1.f, 0.f); break;
		case EGizmoDomain::TD_Z_Axis: domainScale = FVector(0.f, 0.f, 1.f); break;

		case EGizmoDomain::TD_XY_Plane: domainScale = FVector(1.f, 1.f, 0.f); break;
		case EGizmoDomain::TD_YZ_Plane: domainScale = FVector(0.f, 1.f, 1.f); break;
		case EGizmoDomain::TD_XZ_Plane: domainScale = FVector(1.f, 0.f, 1.f); break;

		case EGizmoDomain::TD_XYZ: domainScale = FVector(1.f, 1.f, 1.f); break;
		}
		FVector domainInverseScale = FVector::OneVector - domainScale;

		newScale = newScale.GridSnap(SnappingValue);
		FVector scale = (newScale * domainScale) + (NewComponentTransform.GetScale3D() * domainInverseScale);
		result.SetScale3D(scale);
	}

	return result;
}
