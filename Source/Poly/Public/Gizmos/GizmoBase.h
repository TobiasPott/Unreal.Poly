// Copyright 2020 Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoTypes.h"
#include "GizmoBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGizmoStateUpdatedDelegate, EGizmoType, GizmoType, bool, bTransformInProgress, EGizmoDomain, CurrentDomain);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoDeltaTransformDelegate, bool, bEnded, FTransform, DeltaTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoTranslateTransformDelegate, bool, bEnded, FVector, DeltaTranslation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoScaleTransformDelegate, bool, bEnded, FVector, DeltaScale);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoRotateTransformDelegate, bool, bEnded, FRotator, DeltaRotation);

UCLASS()
class POLY_API AGizmoBase : public AActor
{
	GENERATED_BODY()


	// ToDo: @tpott: Expose SnappedDeltaTransform as Changed events 
	//				This should be solved by only firing the events when the snapped GetDeltaTransform has a value


public:
	// Sets default values for this actor's properties
	AGizmoBase();

	virtual EGizmoType GetGizmoType() const { return EGizmoType::TT_NoTransform; }

	virtual void UpdateGizmoSpace(ETransformSpace SpaceType);

	//Base Gizmo does not affect anything and returns No Delta Transform.
	// This func is overriden by each Transform Gizmo

	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	virtual FTransform GetDeltaTransform(const FVector& LookingVector, const FVector& RayStartPoint
		, const FVector& RayEvndPoint, EGizmoDomain Domain);

	/**
	 * Scales the Gizmo Scene depending on a Reference Point
	 * The scale depends on the Gizmo Screen Space Radius specified,
	 * and the Gizmo Scene Scale Factor.
	 * @param Reference Location - The Location of where the Gizmo is seen (i.e. Camera Location)
	 * @param Reference Look Direction - the direction the reference is looking (i.e. Camera Look Direction)
	 * @param FieldOfView - Field of View of Camera, in Degrees
	*/
	void ScaleGizmoScene(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection, float FieldOfView = 90.f);

	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	EGizmoDomain GetTransformationDomain(class USceneComponent* ComponentHit) const;

	// Returns a Snapped Transform based on how much has been accumulated, the Delta Transform and Snapping Value
	// Also changes the Accumulated Transform based on how much was snapped
	virtual FTransform GetSnappedTransform(FTransform& outCurrentAccumulatedTransform
		, const FTransform& DeltaTransform
		, EGizmoDomain Domain
		, float SnappingValue) const;

	// Snapped Transform per Component is used when we need Absolute Snapping
	// For Scaling, Absolute Snapping is needed and not delta ones 
	// for example, Object Scale (1) and Snapping of (5). Snapping sequence should be 5, 10... and not 6, 11...
	virtual FTransform GetSnappedTransformPerComponent(const FTransform& OldComponentTransform
		, const FTransform& NewComponentTransform
		, EGizmoDomain Domain
		, float SnappingValue) const {
		return NewComponentTransform;
	}

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Calculates the Gizmo Scene Scale. This can be overriden (e.g. by Rotation Gizmo)
	// for additional/optional scaling properties.
	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	virtual FVector CalculateGizmoSceneScale(const FVector& ReferenceLocation, const FVector& ReferenceLookDirection, float FieldOfView);

	// should be called at the start of the GetDeltaTransformation Implemenation
	// returns true if the PrevRays were valid before this Func was called. False if they were invalid (now they should be valid but the next tick should be waited)
	bool AreRaysValid() const;

	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	//should be called at the end of the GetDeltaTransformation Implemenation
	void UpdateRays(const FVector& RayStart, const FVector& RayEnd);

	/**
	 * Adds or modifies an entry to the DomainMap.
	*/
	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	void RegisterDomainComponent(class USceneComponent* Component, EGizmoDomain Domain);

	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	EGizmoDomain GetDomainByTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool& bSuccess);
	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	EGizmoDomain GetDomainByChannel(const ETraceTypeQuery Channel, bool& bSuccess);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	FTransform UpdateDeltaTransform(const bool bEndTransform, const float MaxDistance = 10000);

public:

	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	void SetTransformProgressState(bool bInProgress, EGizmoDomain CurrentDomain);

	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	bool GetTransformProgressState() const { return bTransformInProgress; }

	/**
	 * Delegate that is called when the Transform State is changed (when it changes from
	 * in progress = true to false (and viceversa)
	 * Can be used to Change the meshes and visuals of Gizmo while Transform is in Progress
	 */
	UPROPERTY(BlueprintAssignable, Category = "Gizmo")
	FGizmoStateUpdatedDelegate OnGizmoStateChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoDeltaTransformDelegate TransformChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoTranslateTransformDelegate TranslationChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoRotateTransformDelegate RotationChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gizmo")
	FGizmoScaleTransformDelegate ScaleChanged;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class USceneComponent* RootScene;

	/* Scene Component that will go Under the Root Scene
	 * This is so that we can Scale all the things under it without Scaling the Actor itself (i.e. root component)
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class USceneComponent* ScalingScene;

	// The Hit Box for the X-Axis Direction Transform
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class UBoxComponent* X_AxisBox;

	// The Hit Box for the X-Axis Direction Transform
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class UBoxComponent* Y_AxisBox;

	// The Hit Box for the X-Axis Direction Transform
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	class UBoxComponent* Z_AxisBox;

	// Used to calculate the distance the rays have travelled
	FVector PreviousRayStartPoint;
	FVector PreviousRayEndPoint;

	/* The Radius of the Arc (FOV) that the Camera covers. The bigger the value, the smaller the Gizmo would look. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	int32 PlayerIndex = 0;
	UPROPERTY()
	FName InputAction = EKeys::LeftMouseButton.GetFName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gizmo")
	bool bEnableScreenSpaceScale = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo")
	float GizmoSceneScaleFactor;

	/* The Radius of the Arc (FOV) that the Camera covers. The bigger the value, the smaller the Gizmo would look. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo")
	float CameraArcRadius;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	EGizmoDomain ActiveDomain = EGizmoDomain::TD_None;


private:
	// Maps the Box Component to their Respective Domain
	TMap<class UShapeComponent*, EGizmoDomain> DomainMap;

	//Whether Transform is in Progress or Not 
	bool bTransformInProgress;

protected:

	//bool to check whether the PrevRay vectors have been set
	bool bIsPrevRayValid;

protected:


	UFUNCTION(BlueprintCallable, Category = "Gizmo")
	void SetEnableConsumeInput(const bool bInEnable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gizmo")
	void OnInputKey_Pressed(FKey InKey);
	virtual void OnInputKey_Pressed_Implementation(FKey InKey);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gizmo")
	void OnInputKey_Released(FKey InKey);
	virtual void OnInputKey_Released_Implementation(FKey InKey);
	UFUNCTION()
	virtual void OnMouseX(float AxisValue);
	UFUNCTION()
	virtual void OnMouseY(float AxisValue);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gizmo")
	void OnMouse2D(FVector AxisValue);
	virtual void OnMouse2D_Implementation(FVector AxisValue);


};

