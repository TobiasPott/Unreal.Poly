// Copyright 2020 Juan Marcelo Portillo. All Rights Reserved.


#include "Gizmos/GizmoBase.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGizmoBase::AGizmoBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	this->SetRootComponent(RootScene);

	ScalingScene = CreateDefaultSubobject<USceneComponent>(TEXT("ScalingScene"));
	ScalingScene->SetupAttachment(RootScene);

	X_AxisBox = CreateDefaultSubobject<UBoxComponent>(TEXT("X Axis Box"));
	X_AxisBox->SetComponentTickEnabled(false);
	Y_AxisBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Y Axis Box"));
	Y_AxisBox->SetComponentTickEnabled(false);
	Z_AxisBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Z Axis Box"));
	Z_AxisBox->SetComponentTickEnabled(false);

	X_AxisBox->SetupAttachment(ScalingScene);
	Y_AxisBox->SetupAttachment(ScalingScene);
	Z_AxisBox->SetupAttachment(ScalingScene);

	RegisterDomainComponent(X_AxisBox, EGizmoDomain::TD_X_Axis);
	RegisterDomainComponent(Y_AxisBox, EGizmoDomain::TD_Y_Axis);
	RegisterDomainComponent(Z_AxisBox, EGizmoDomain::TD_Z_Axis);

	GizmoSceneScaleFactor = 0.1f;
	CameraArcRadius = 150.f;

	PreviousRayStartPoint = FVector::ZeroVector;
	PreviousRayEndPoint = FVector::ZeroVector;

	bTransformInProgress = false;
	bIsPrevRayValid = false;
}

void AGizmoBase::UpdateGizmoSpace(ETransformSpace SpaceType)
{
	switch (SpaceType)
	{
	case ETransformSpace::ST_Local:
		SetActorRelativeRotation(FQuat(EForceInit::ForceInit));
		break;
	case ETransformSpace::ST_World:
		SetActorRotation(FQuat(EForceInit::ForceInit), ETeleportType::TeleportPhysics);
		break;
	}
}

//Base Gizmo does not affect anything and returns No Delta Transform.
// This func is overriden by each Transform Gizmo

FTransform AGizmoBase::GetDeltaTransform(const FVector& LookingVector
	, const FVector& RayStartPoint, const FVector& RayEndPoint
	, EGizmoDomain Domain)
{
	FTransform deltaTransform;
	deltaTransform.SetScale3D(FVector::ZeroVector);
	return deltaTransform;
}

void AGizmoBase::ScaleGizmoScene(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection, float FieldOfView)
{
	FVector Scale = CalculateGizmoSceneScale(ReferenceLocation, ReferenceLookDirection, FieldOfView);
	//UE_LOG(LogGizmo, Warning, TEXT("Scale: %s"), *Scale.ToString());
	if (ScalingScene)
		ScalingScene->SetWorldScale3D(Scale);
}

FTransform AGizmoBase::GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
	, const FTransform& DeltaTransform
	, EGizmoDomain Domain
	, float SnappingValue) const
{
	return DeltaTransform;
}

EGizmoDomain AGizmoBase::GetTransformationDomain(USceneComponent* ComponentHit) const
{
	if (!ComponentHit) return EGizmoDomain::TD_None;

	if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(ComponentHit))
	{
		if (const EGizmoDomain* pDomain = DomainMap.Find(ShapeComponent))
			return *pDomain;
	}
	else
		UE_LOG(LogGizmo, Warning, TEXT("Failed to Get Domain! Component Hit is not a Shape Component. %s"), *ComponentHit->GetName());

	return EGizmoDomain::TD_None;
}

FVector AGizmoBase::CalculateGizmoSceneScale(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection, float FieldOfView)
{
	FVector deltaLocation = (GetActorLocation() - ReferenceLocation);
	float distance = deltaLocation.ProjectOnTo(ReferenceLookDirection).Size();
	float scaleView = (distance * FMath::Sin(FMath::DegreesToRadians(FieldOfView))) / CameraArcRadius;
	scaleView *= GizmoSceneScaleFactor;
	return FVector(scaleView);
}

bool AGizmoBase::AreRaysValid() const
{
	return bIsPrevRayValid;
}

void AGizmoBase::UpdateRays(const FVector& RayStart, const FVector& RayEnd)
{
	PreviousRayStartPoint = RayStart;
	PreviousRayEndPoint = RayEnd;
	bIsPrevRayValid = true;
}

void AGizmoBase::RegisterDomainComponent(USceneComponent* Component
	, EGizmoDomain Domain)
{
	if (!Component) return;

	if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Component))
		DomainMap.Add(ShapeComponent, Domain);
	else
		UE_LOG(LogGizmo, Warning, TEXT("Failed to Register Component! Component is not a Shape Component %s"), *Component->GetName());
}

void AGizmoBase::SetInputEnabled(bool bInEnabled)
{
	if (bInEnabled)
		this->EnableInput(UGameplayStatics::GetPlayerController(this, this->PlayerIndex));
	else
		this->DisableInput(UGameplayStatics::GetPlayerController(this, this->PlayerIndex));
}

void AGizmoBase::SetTransformProgressState(bool bInProgress
	, EGizmoDomain CurrentDomain)
{
	if (bInProgress != bTransformInProgress)
	{
		bIsPrevRayValid = false; //set this so that we don't get an invalid delta value
		bTransformInProgress = bInProgress;
		OnGizmoStateChange.Broadcast(GetGizmoType(), bTransformInProgress, CurrentDomain);
	}
}
