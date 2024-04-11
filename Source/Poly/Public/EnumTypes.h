// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EObjectSelectionMode : uint8 {
	None = 0, // disable change to selection
	Add = 1, // add objects to selection
	Remove = 2, // remove objects from selection
	Toggle = 3 // add items not  yet selected, remove items already selected (one iteration)
};