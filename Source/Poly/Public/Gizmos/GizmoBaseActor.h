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
#include "GizmoBaseActor.generated.h"

UCLASS()
class POLY_API AGizmoBaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGizmoBaseActor();


	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	EGizmoExtType Type = EGizmoExtType::GET_NoTransform;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	ETransformSpace Space = ETransformSpace::TS_World;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	FName SelectorName = "Default";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UClass> SelectClass = AActor::StaticClass();

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	TArray<AActor*> Selection;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	inline ETransformSpace InverseSpace() { return Space == ETransformSpace::TS_Local ? ETransformSpace::TS_World : ETransformSpace::TS_Local; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	inline bool IsLocalSpace() { return Space == ETransformSpace::TS_Local; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	inline bool IsWorldSpace() { return Space == ETransformSpace::TS_World; };


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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setup")
	void CreateElementsCore(AElementsGizmo*& OutSelectCore);
	virtual void CreateElementsCore_Implementation(AElementsGizmo*& OutElementsCore);


	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Translate_TranslationChanged(bool bEnded, FVector DeltaTranslation);
	void Translate_TranslationChanged_Implementation(bool bEnded, FVector DeltaTranslation);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Translate_TransformEnded(bool bEnded, FTransform DeltaTransform);
	void Translate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Rotate_RotationChanged(bool bEnded, FRotator DeltaRotation);
	void Rotate_RotationChanged_Implementation(bool bEnded, FRotator DeltaRotation);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Rotate_TransformEnded(bool bEnded, FTransform DeltaTransform);
	void Rotate_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Scale_ScaleChanged(bool bEnded, FVector DeltaScale);
	void Scale_ScaleChanged_Implementation(bool bEnded, FVector DeltaScale);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Protected|Core Events")
	void Scale_TransformEnded(bool bEnded, FTransform DeltaTransform);
	void Scale_TransformEnded_Implementation(bool bEnded, FTransform DeltaTransform);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Select_Finished(UActorSelectionRequest* Request, bool bSuccess);
	void Select_Finished_Implementation(UActorSelectionRequest* Request, bool bSuccess);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Protected|Core Events")
	void Elements_Finished(AElementsGizmo* Core);
	void Elements_Finished_Implementation(AElementsGizmo* Core);



	UFUNCTION(BlueprintCallable, Category = "Intern")
	void SetupCores();

	UFUNCTION(BlueprintCallable, Category = "Intern")
	void TransformSelection(FTransform DeltaTransform, bool bInLocalSpace);
	UFUNCTION(BlueprintCallable, Category = "Intern")
	void TransformCore(FTransform DeltaTransform, bool bInLocalSpace, AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Intern")
	void UpdateGizmoSpace(ETransformSpace InSpace);


	UFUNCTION(BlueprintCallable, Category = "Intern")
	FRotator GetRotationFromSelection();


public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

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
