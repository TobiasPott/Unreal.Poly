// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gizmos/GizmoTypes.h"
#include "Gizmos/GizmoCore.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Gizmos/SelectGizmo.h"
#include "Gizmos/ElementsGizmo.h"
#include "GizmoBaseActor_Intern.generated.h"

UCLASS()
class POLY_API AGizmoBaseActor_Intern : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGizmoBaseActor_Intern() { };

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateTranslateCore(ATranslateGizmo*& OutTranslateCore);
	virtual void CreateTranslateCore_Implementation(ATranslateGizmo*& OutTranslateCore) { };
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateRotateCore(ARotateGizmo*& OutRotateCore);
	virtual void CreateRotateCore_Implementation(ARotateGizmo*& OutRotateCore) { };
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateScaleCore(AScaleGizmo*& OutScaleCore);
	virtual void CreateScaleCore_Implementation(AScaleGizmo*& OutScaleCore) { };
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateSelectCore(ASelectGizmo*& OutSelectCore);
	virtual void CreateSelectCore_Implementation(ASelectGizmo*& OutSelectCore) { };
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateElementsCore(AElementsGizmo*& OutSelectCore);
	virtual void CreateElementsCore_Implementation(AElementsGizmo*& OutElementsCore) { };


	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Translate_TranslationChanged(bool bEnded, FVector DeltaTranslation);
	virtual void Translate_TranslationChanged_Implementation(bool bEnded, FVector DeltaTranslation) { };
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Translate_TransformEnded(bool bEnded, FTransform DeltaTransform);
	virtual void Translate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform) { };

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Rotate_RotationChanged(bool bEnded, FRotator DeltaRotation);
	virtual void Rotate_RotationChanged_Implementation(bool bEnded, FRotator DeltaRotation) { };
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Rotate_TransformEnded(bool bEnded, FTransform DeltaTransform);
	virtual void Rotate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform) { };

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Scale_ScaleChanged(bool bEnded, FVector DeltaScale);
	virtual void Scale_ScaleChanged_Implementation(bool bEnded, FVector DeltaScale) { };
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Protected|Core Events")
	void Scale_TransformEnded(bool bEnded, FTransform DeltaTransform);
	virtual void Scale_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform) { };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Select_Finished(UActorSelectionRequest* Request, bool bSuccess);
	virtual void Select_Finished_Implementation(UActorSelectionRequest* Request, bool bSuccess) { };


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Elements_Finished(AElementsGizmo* Core);
	virtual void Elements_Finished_Implementation(AElementsGizmo* Core) { };

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TObjectPtr<ATranslateGizmo> TranslateCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TSubclassOf<ATranslateGizmo> TranslateCoreClass = ATranslateGizmo::StaticClass();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TObjectPtr<ARotateGizmo> RotateCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TSubclassOf<ARotateGizmo> RotateCoreClass = ARotateGizmo::StaticClass();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TObjectPtr<AScaleGizmo> ScaleCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TSubclassOf<AScaleGizmo> ScaleCoreClass = AScaleGizmo::StaticClass();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TObjectPtr<ASelectGizmo> SelectCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TSubclassOf<ASelectGizmo> SelectCoreClass = ASelectGizmo::StaticClass();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TObjectPtr<AElementsGizmo> ElementsCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Cores")
	TSubclassOf<AElementsGizmo> ElementsCoreClass = AElementsGizmo::StaticClass();

};
