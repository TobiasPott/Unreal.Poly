// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSceneChangedSignature, ASceneActor*, Scene);


UCLASS(BlueprintType)
class POLY_API ASceneActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> DefaultSceneRoot;


public:	
	// Sets default values for this actor's properties
	ASceneActor();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Scene")
	FName Name = FName("NewScene");
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scene")
	TArray<AActor*> AssociatedActors;  // contains any actors which should be retrievable through the scene instance

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() { return AssociatedActors.IsEmpty(); }


	UFUNCTION(BlueprintCallable, Category = "Scene")
	void Add(AActor* InActor, bool bAttach = true);

	UFUNCTION(BlueprintCallable, Category = "Scene")
	void Remove(AActor* InActor, bool bDetach = true);



	UFUNCTION()
	void OnActorDestroyed(AActor* DestroyedActor);
	UFUNCTION()
	void OnSceneChanged() { if (SceneChanged.IsBound()) SceneChanged.Broadcast(this); }




	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Scene")
	FSceneChangedSignature SceneChanged;

};
