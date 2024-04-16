// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetTypes.generated.h"


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