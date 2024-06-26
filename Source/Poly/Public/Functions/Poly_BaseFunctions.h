// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Selection/SelectionRequest.h"
#include "Poly_BaseFunctions.generated.h"

/**
 *
 */
UCLASS()
class POLY_API UPoly_BaseFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Poly|Base", meta = (WorldContext = "WorldContext", DeterminesOutputType = "InClass", DynamicOutputParam = "OutActor"))
	static void GetOrCreateActor(const UObject* WorldContext, TSubclassOf<AActor> InClass, AActor*& OutActor);



	UFUNCTION(BlueprintCallable, Category = "Poly|Base", meta = (DisplayName = "Set Actor Hidden (w Collision)"))
	static void SetActorHidden(AActor* Target, const bool bHiddenInGame = false);






	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities")
	static FTransform Transform_Identity() { return FTransform::Identity; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities", meta = (DisplayName = "Translation Only (Transform)"))
	static FTransform Transform_TranslationOnly(const FVector& InTranslation) { return FTransform(FRotator::ZeroRotator, InTranslation, FVector::ZeroVector); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities", meta = (DisplayName = "Rotation Only (Transform)"))
	static FTransform Transform_RotationOnly(const FRotator& InRotation) { return FTransform(InRotation, FVector::ZeroVector, FVector::ZeroVector); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities", meta = (DisplayName = "Scale Only (Transform)"))
	static FTransform Transform_ScaleOnly(const FVector& InScale) { return FTransform(FRotator::ZeroRotator, FVector::ZeroVector, InScale); }

};
