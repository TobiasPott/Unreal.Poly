// Copyright 2020 Juan Marcelo Portillo. All Rights Reserved.


#include "Gizmos/GizmoCoreActor.h"
#include "Functions/Poly_UIFunctions.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGizmoCoreActor::AGizmoCoreActor()
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

	PreviousRayStartPoint = FVector::ZeroVector;
	PreviousRayEndPoint = FVector::ZeroVector;
	PreviousViewScale = FVector::OneVector;

	bTransformInProgress = false;
	bIsPrevRayValid = false;

}

// Called when the game starts or when spawned
void AGizmoCoreActor::BeginPlay()
{
	Super::BeginPlay();

	this->EnableInput(UGameplayStatics::GetPlayerController(this, this->PlayerIndex));

	FInputActionBinding& IKA_Pressed = InputComponent->BindAction(InputAction, EInputEvent::IE_Pressed, this, &AGizmoCoreActor::OnInputKey_Pressed);
	IKA_Pressed.bConsumeInput = false;
	IKA_Pressed.bExecuteWhenPaused = true;
	FInputActionBinding& IKA_Released = InputComponent->BindAction(InputAction, EInputEvent::IE_Released, this, &AGizmoCoreActor::OnInputKey_Released);
	IKA_Released.bConsumeInput = false;
	IKA_Released.bExecuteWhenPaused = true;

}
void AGizmoCoreActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	this->ScaleToScreenSpace();
}
void AGizmoCoreActor::UpdateGizmoSpace(ETransformSpace SpaceType)
{
	switch (SpaceType)
	{
	case ETransformSpace::TS_Local:
		SetActorRelativeRotation(FQuat(EForceInit::ForceInit));
		break;
	case ETransformSpace::TS_World:
		SetActorRotation(FQuat(EForceInit::ForceInit), ETeleportType::TeleportPhysics);
		break;
	}
}

//Base Gizmo does not affect anything and returns No Delta Transform.
// This func is overriden by each Transform Gizmo

FTransform AGizmoCoreActor::GetDeltaTransform(const FVector& LookingVector
	, const FVector& RayStartPoint, const FVector& RayEndPoint
	, EGizmoDomain Domain)
{
	FTransform deltaTransform;
	deltaTransform.SetScale3D(FVector::ZeroVector);
	return deltaTransform;
}

void AGizmoCoreActor::ScaleGizmoScene(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection, float FieldOfView)
{
	FVector Scale = CalculateGizmoSceneScale(ReferenceLocation, ReferenceLookDirection, FieldOfView);
	//UE_LOG(LogGizmo, Warning, TEXT("Scale: %s"), *Scale.ToString());
	if (ScalingScene)
		ScalingScene->SetWorldScale3D(Scale);
}

FTransform AGizmoCoreActor::GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
	, const FTransform& DeltaTransform
	, EGizmoDomain Domain
	, float SnappingValue) const
{
	return DeltaTransform;
}

EGizmoDomain AGizmoCoreActor::GetTransformationDomain(USceneComponent* ComponentHit) const
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

FVector AGizmoCoreActor::CalculateGizmoSceneScale(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection, float FieldOfView)
{
	FVector DeltaLocation = (GetActorLocation() - ReferenceLocation);
	float Distance = DeltaLocation.ProjectOnTo(ReferenceLookDirection).Size();
	float ScaleView = (Distance * FMath::Sin(FMath::DegreesToRadians(FieldOfView))) / CameraArcRadius;
	ScaleView *= GizmoSceneScaleFactor;
	FVector CalculatedScale = FVector(ScaleView);

	FVector CurrentRotationViewScale = PreviousViewScale;

	bool bInProgress = GetTransformProgressState();
	if (!bInProgress)
	{
		FVector deltaLocationInv = ReferenceLocation - GetActorLocation();
		// ToDo: @tpott: transfer this view angle dependend scale to translate and scale gizmo (I like the axis flip for rotation)
		CurrentRotationViewScale = FVector(
			(FVector::DotProduct(GetActorForwardVector(), deltaLocationInv) >= 0) ? 1.f : -1.f,
			(FVector::DotProduct(GetActorRightVector(), deltaLocationInv) >= 0) ? 1.f : -1.f,
			(FVector::DotProduct(GetActorUpVector(), deltaLocationInv) >= 0) ? 1.f : -1.f
		);

		PreviousViewScale = CurrentRotationViewScale;
	}

	CalculatedScale *= CurrentRotationViewScale;
	return CalculatedScale;
}

bool AGizmoCoreActor::AreRaysValid() const
{
	return bIsPrevRayValid;
}

void AGizmoCoreActor::UpdateRays(const FVector& RayStart, const FVector& RayEnd)
{
	if (!bIsPrevRayValid)
	{
		FirstRayStartPoint = RayStart;
		FirstRayEndPoint = RayEnd;
	}
	PreviousRayStartPoint = RayStart;
	PreviousRayEndPoint = RayEnd;
	bIsPrevRayValid = true;
}

void AGizmoCoreActor::RegisterDomainComponent(USceneComponent* Component
	, EGizmoDomain Domain)
{
	if (!Component) return;

	if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Component))
		DomainMap.Add(ShapeComponent, Domain);
	else
		UE_LOG(LogGizmo, Warning, TEXT("Failed to Register Component! Component is not a Shape Component %s"), *Component->GetName());
}

EGizmoDomain AGizmoCoreActor::GetDomainByTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool& bSuccess)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, this->PlayerIndex);

	FHitResult HitResult;
	if (PC->GetHitResultUnderCursorForObjects(ObjectTypes, true, HitResult))
	{
		const EGizmoDomain Domain = GetTransformationDomain(HitResult.GetComponent());
		bSuccess = Domain != EGizmoDomain::TD_None;
		return Domain;
	}
	bSuccess = false;
	return EGizmoDomain::TD_None;
}

EGizmoDomain AGizmoCoreActor::GetDomainByChannel(const ETraceTypeQuery Channel, bool& bSuccess)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, this->PlayerIndex);

	FHitResult HitResult;
	if (PC->GetHitResultUnderCursorByChannel(Channel, true, HitResult))
	{
		const EGizmoDomain Domain = GetTransformationDomain(HitResult.GetComponent());
		bSuccess = Domain != EGizmoDomain::TD_None;
		return GetTransformationDomain(HitResult.GetComponent());
	}
	bSuccess = false;
	return EGizmoDomain::TD_None;
}

FTransform AGizmoCoreActor::UpdateDeltaTransform(const bool bEndTransform, const float MaxDistance)
{

	FTransform Delta = FTransform::Identity;
	FVector Start, End;
	if (this->ActiveDomain != EGizmoDomain::TD_None)
	{
		const FVector LookVector = UGameplayStatics::GetPlayerCameraManager(this, this->PlayerIndex)->GetActorForwardVector();
		if (UPoly_UIFunctions::GetMouseRaySegment(this, this->PlayerIndex, Start, End, MaxDistance))
		{
			Delta = GetDeltaTransform(LookVector, Start, End, this->ActiveDomain);
		}
	}
	if (bEndTransform)
	{
		// reset rays to first rays
		const FVector LookVector = UGameplayStatics::GetPlayerCameraManager(this, this->PlayerIndex)->GetActorForwardVector();
		GetDeltaTransform(LookVector, FirstRayStartPoint, FirstRayEndPoint, this->ActiveDomain);
		// get delta between first ray and current (last) ray
		Delta = GetDeltaTransform(LookVector, Start, End, this->ActiveDomain);
		// always fire events if bEndTransform is sets
		this->OnTransformChanged(true, Delta);
		this->OnTransformEnded(Delta);

		this->SetTransformProgressState(false, EGizmoDomain::TD_None);
		this->ActiveDomain = EGizmoDomain::TD_None;

	}
	return Delta;
}

void AGizmoCoreActor::SetTransformProgressState(bool bInProgress, EGizmoDomain CurrentDomain)
{
	if (bInProgress != bTransformInProgress)
	{
		bIsPrevRayValid = false; //set this so that we don't get an invalid delta value
		bTransformInProgress = bInProgress;
		OnGizmoStateChange.Broadcast(GetGizmoType(), bTransformInProgress, CurrentDomain);
	}
}




void AGizmoCoreActor::SetEnableScaleToScreenSpace(const bool bInEnable)
{
	this->bEnableScaleToScreenSpace = bInEnable;
	this->ScaleToScreenSpace();
}

void AGizmoCoreActor::ScaleToScreenSpace()
{
	if (bEnableScaleToScreenSpace)
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, this->PlayerIndex);
		FVector NewScale = this->CalculateGizmoSceneScale(CameraManager->GetCameraLocation(), CameraManager->GetActorForwardVector(), 75.0f);
		this->ScalingScene->SetRelativeScale3D(NewScale);
	}
	else
	{
		this->ScalingScene->SetRelativeScale3D(FVector::OneVector);
	}
}

void AGizmoCoreActor::SetEnableConsumeInput(const bool bInEnable)
{
	//bIsEnabled = bInEnable;
	if (bInEnable)
	{
		// bind mouse axes events (to conume their input from other receivers)
		FInputVectorAxisBinding& Mouse2D_Axis = InputComponent->BindVectorAxis("Mouse2D");
		Mouse2D_Axis.AxisDelegate.BindDelegate(this, &AGizmoCoreActor::OnMouse2D);
		Mouse2D_Axis.bConsumeInput = true;
		Mouse2D_Axis.bExecuteWhenPaused = true;

		// bind primary input key pressed/released events
		InputComponent->AxisKeyBindings.Reset(0);
		FInputAxisKeyBinding& MouseX_Axis = InputComponent->BindAxisKey("MouseX");
		MouseX_Axis.AxisDelegate.BindDelegate(this, &AGizmoCoreActor::OnMouseX);
		MouseX_Axis.bConsumeInput = true;
		MouseX_Axis.bExecuteWhenPaused = true;

		FInputAxisKeyBinding& MouseY_Axis = InputComponent->BindAxisKey("MouseY");
		MouseY_Axis.AxisDelegate.BindDelegate(this, &AGizmoCoreActor::OnMouseY);
		MouseY_Axis.bConsumeInput = true;
		MouseY_Axis.bExecuteWhenPaused = true;

	}
	else
	{
		InputComponent->AxisKeyBindings.Reset(0);
		InputComponent->VectorAxisBindings.Reset(0);
	}

}

void AGizmoCoreActor::OnInputKey_Pressed_Implementation(FKey InKey)
{
	bool bSuccess;
	this->ActiveDomain = this->GetDomainByChannel(ETraceTypeQuery::TraceTypeQuery3, bSuccess);
	this->bInputKeyPressCaptured = bSuccess;
	this->SetEnableConsumeInput(bSuccess);

	this->SetTransformProgressState(true, this->ActiveDomain);
	this->UpdateDeltaTransform(false);
	this->ScaleToScreenSpace();
}

void AGizmoCoreActor::OnInputKey_Released_Implementation(FKey InKey)
{
	if (bInputKeyPressCaptured)
	{
		// update delta transform (with end flag set)
		this->UpdateDeltaTransform(true);
		// disable input consumption (including input key pressed/released)
		this->SetEnableConsumeInput(false);
	}

	// triggers gizmo state change if necessary
	this->SetTransformProgressState(false, EGizmoDomain::TD_None);
	bInputKeyPressCaptured = false;
}

void AGizmoCoreActor::OnMouseX(float AxisValue)
{
	// Only there to consume mouse axis input before pawn or player controller do for camera (i.e.: lock camera)
}

void AGizmoCoreActor::OnMouseY(float AxisValue)
{
	// Only there to consume mouse axis input before pawn or player controller do for camera (i.e.: lock camera)
}

void AGizmoCoreActor::OnMouse2D_Implementation(FVector AxisValue)
{
	this->UpdateDeltaTransform(false);
}
