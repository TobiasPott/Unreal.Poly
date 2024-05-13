// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GizmoTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGizmo, Log, All);



UENUM(BlueprintType)
enum class EGizmoType : uint8
{
	GT_NoTransform			UMETA(DisplayName = "None"),
	GT_Translation			UMETA(DisplayName = "Translation"),
	GT_Rotation				UMETA(DisplayName = "Rotation"),
	GT_Scale				UMETA(DisplayName = "Scale")
};

UENUM(BlueprintType)
enum class EGizmoExtType : uint8
{
	GET_NoTransform			UMETA(DisplayName = "None"),
	GET_Translation			UMETA(DisplayName = "Translation"),
	GET_Rotation			UMETA(DisplayName = "Rotation"),
	GET_Scale				UMETA(DisplayName = "Scale"),
	GET_Select				UMETA(DisplayName = "Select"),
	GET_Elements			UMETA(DisplayName = "Elements"),
	GET_TRS					UMETA(DisplayName = "TRS"), // declared for late use, as a combiend translate, rotate & scale gizmo
};


UENUM(BlueprintType)
enum class ETransformSpace : uint8
{
	TS_None				UMETA(DisplayName = "None"),
	TS_Local			UMETA(DisplayName = "Local Space"),
	TS_World			UMETA(DisplayName = "World Space"),
};

UENUM(BlueprintType) 
enum class EGizmoDomain : uint8
{
	TD_None				UMETA(DisplayName = "None"),

	TD_X_Axis			UMETA(DisplayName = "X Axis"),
	TD_Y_Axis			UMETA(DisplayName = "Y Axis"),
	TD_Z_Axis			UMETA(DisplayName = "Z Axis"),

	TD_XY_Plane			UMETA(DisplayName = "XY Plane"),
	TD_YZ_Plane			UMETA(DisplayName = "YZ Plane"),
	TD_XZ_Plane			UMETA(DisplayName = "XZ Plane"),

	TD_XYZ				UMETA(DisplayName = "XYZ"),

};

UENUM(BlueprintType)
enum class EGizmoPivotSelectionSource : uint8
{
	PSS_NONE = 0				UMETA(DisplayName = "None"),
	PSS_Actor = 1			UMETA(DisplayName = "Actors"),
	PSS_Elements = 2			UMETA(DisplayName = "Elements"),
};

UENUM(BlueprintType)
enum class EGizmoPivotSource : uint8
{
	//PS_NONE = 0				UMETA(Hidden),
	PS_Identity = 0			UMETA(DisplayName = "Identity"),
	PS_Self = 1				UMETA(DisplayName = "Self"),
	PS_First = 2			UMETA(DisplayName = "First (of selection)"), // defaults to identity of unsupported
	PS_Last = 3				UMETA(DisplayName = "Last (of selection)"), // defaults to identity of unsupported
	PS_Center = 4			UMETA(DisplayName = "Center (of selection)"), // defaults to identity of unsupported
	PS_Custom = 5			UMETA(DisplayName = "Custom"), // defaults to set pivot of unsupported
};
//ENUM_CLASS_FLAGS(EGizmoPivotSource);


UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGizmoDomainMask : uint8
{
	GDM_NONE = 0			UMETA(Hidden),
	GDM_X_Axis = 1			UMETA(DisplayName = "X Axis"),
	GDM_Y_Axis = 2			UMETA(DisplayName = "Y Axis"),
	GDM_Z_Axis = 4			UMETA(DisplayName = "Z Axis"),
};
ENUM_CLASS_FLAGS(EGizmoDomainMask);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGizmoStateUpdatedDelegate, EGizmoType, GizmoType, bool, bTransformInProgress, EGizmoDomain, CurrentDomain);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoDeltaTransformDelegate, bool, bEnded, FTransform, DeltaTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoTranslateTransformDelegate, bool, bEnded, FVector, DeltaTranslation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoScaleTransformDelegate, bool, bEnded, FVector, DeltaScale);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGizmoRotateTransformDelegate, bool, bEnded, FRotator, DeltaRotation);




USTRUCT(Blueprintable)
struct POLY_API FGizmoPivot
{
	GENERATED_BODY()



public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FRotator Orientation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	ETransformSpace Space = ETransformSpace::TS_World;


	inline FVector GetLocation() const { return Location; };
	inline FRotator GetOrientation() const { return Orientation; };
	inline ETransformSpace GetSpace() const { return Space; };
	inline ETransformSpace GetInverseSpace() const { return Space == ETransformSpace::TS_Local ? ETransformSpace::TS_World : ETransformSpace::TS_Local; };
	inline bool IsLocalSpace() const { return Space == ETransformSpace::TS_Local; };
	inline bool IsWorldSpace() const { return Space == ETransformSpace::TS_World; };

};



