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
#include "GizmoBaseActor.generated.h"

UCLASS()
class POLY_API AGizmoBaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGizmoBaseActor();

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateTranslateCore(ATranslateGizmo*& OutTranslateCore);
	virtual void CreateTranslateCore_Implementation(ATranslateGizmo*& OutTranslateCore);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateRotateCore(ARotateGizmo*& OutRotateCore);
	virtual void CreateRotateCore_Implementation(ARotateGizmo*& OutRotateCore);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateScaleCore(AScaleGizmo*& OutScaleCore);
	virtual void CreateScaleCore_Implementation(AScaleGizmo*& OutScaleCore);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateSelectCore(ASelectGizmo*& OutSelectCore);
	virtual void CreateSelectCore_Implementation(ASelectGizmo*& OutSelectCore);

	///** Please add a function description */
	//UFUNCTION(BlueprintCallable, Category = "Protected/Event Listener")
	//void Translate_TranslationChanged(bool bEnded, FVector DeltaTranslation);
	//UFUNCTION(BlueprintCallable, Category = "Protected/Event Listener")
	//void Translate_TransformEnded(bool bEnded, FTransform DeltaTransform);

	///** Please add a function description */
	//UFUNCTION(BlueprintCallable, Category = "Protected/Event Listener")
	//void Rotate_RotationChanged(bool bEnded, FRotator DeltaRotation);
	//UFUNCTION(BlueprintCallable, Category = "Protected/Event Listener")
	//void Rotate_TransformEnded(bool bEnded, FTransform DeltaTransform);

	///** Please add a function description */
	//UFUNCTION(BlueprintCallable, Category = "Protected/Event Listener")
	//void Scale_ScaleChanged(bool bEnded, FVector DeltaScale);
	//UFUNCTION(BlueprintCallable, Category = "Protected/Event Listener")
	//void Scale_TransformEnded(bool bEnded, FTransform DeltaTransform);

	///** Please add a function description */
	//UFUNCTION(BlueprintCallable, Category = "Protected/ Event Listener")
	//void Select_Finished(UActorSelectionRequest* Request, bool bSuccess);


public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TObjectPtr<ATranslateGizmo> TranslateCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TSubclassOf<ATranslateGizmo> TranslateCoreClass;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TObjectPtr<ARotateGizmo> RotateCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TSubclassOf<ARotateGizmo> RotateCoreClass;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TObjectPtr<AScaleGizmo> ScaleCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TSubclassOf<AScaleGizmo> ScaleCoreClass;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TObjectPtr<ASelectGizmo> SelectCore;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cores")
	TSubclassOf<ASelectGizmo> SelectCoreClass;
};
