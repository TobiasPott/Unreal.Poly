// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetTypes.generated.h"

// UI Icon resources
// https://fonts.google.com/icons?icon.category=Common+actions&icon.style=Rounded&icon.size=96&icon.color=%23FFFFFF

UENUM(BlueprintType)
enum class ESelectionRequestMode : uint8 {
	Click = 0,
	Marquee,
	Trace
	// ClickRepeat, 
	// MarqueeRepeat#
	// // posssible variant mode to perform consecutive clicks (perhaps with counter to allow specific numbers of selection to be done)
	//		repeat modes are more complex as it should count selected objects instead of attempts  (auto-retry)
	//		this might be implementable as a subtype of the oneshot selector type
};

UENUM(BlueprintType)
enum class EObjectFieldNativeDataType : uint8 {
	Object = 0,
	Actor,
	Component,
};

UENUM(BlueprintType)
enum class EPropertyNativeDataType : uint8 {
	Boolean = 0,
	Byte,
	Integer,
	IntVector2, // usses IntVector as in & out type
	IntVector3,
	IntVector4,
	Float,
	Vector2,
	Vector3,
	Vector4,
	Rotator, // is handled as -180 to 180 ranged Vector3 with rotator labeling
	String,
	Text, // localizable, thus unclear when to use this
	// ToDo: @tpott: Consider adding multiline value type (or should this be configured on the widget/UI side)?
	Transform,
	// ToDo: @tpott: Object is a special case which should be divided into significant subtypes (e.g. Actor, Component, DynamicMesh, etc.)
	//				 subtypes might be indicated in a different ways (e.g. widget specialisation)
	Object
};


/**
 *
 */
USTRUCT(Blueprintable)
struct POLY_API FNamedPropertyWidgetRef
{
	GENERATED_BODY()

public:
	// base room arguments
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName Name = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EPropertyNativeDataType DataType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* Widget = nullptr;

};