// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(LogPoly, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPolyDebug, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPolyTemp, Log, All);


UENUM(BlueprintType)
enum class EPolyLogCategory : uint8 {
	None = 0, // disable change to selection
	Default = 1,
	Debug = 2,
	Temp = 3,
};
