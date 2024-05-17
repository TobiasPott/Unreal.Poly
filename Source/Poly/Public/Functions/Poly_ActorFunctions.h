// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Gizmos/GizmoTypes.h"
#include "UDynamicMesh.h"
#include "Poly_ActorFunctions.generated.h"



/**
 *
 */
UCLASS()
class POLY_API UPoly_ActorFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Create a component or subobject that will be instanced inside all instances of this class.
	 * @param	TargetActor					
	 * @param	TReturnType					Class of return type, all overrides must be of this type
	 * @param	SubobjectName				Name of the new component
	 * @param	Mobility					
	 * @param	bTransient					True if the component is being assigned to a transient property. This does not make the component itself transient, but does stop it from inheriting parent defaults
	 */
	template<class TReturnType>
	static TReturnType* CreateDefaultSceneComponent(AActor* TargetActor, FName SubobjectName, EComponentMobility::Type Mobility = EComponentMobility::Static, bool bTransient = false)
	{
		UClass* ReturnType = TReturnType::StaticClass();
		TReturnType* Result = static_cast<TReturnType*>(TargetActor->CreateDefaultSubobject(SubobjectName, ReturnType, ReturnType, /*bIsRequired =*/ true, bTransient));

		Result->SetMobility(Mobility);
		TargetActor->SetRootComponent(Result);

		return Result;
	}



	UFUNCTION(BlueprintCallable, Category = "Actor")
	static bool GetDynamicMesh(AActor* TargetActor, UDynamicMesh*& Mesh);

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static FVector GetLocation(AActor* TargetActor, const ETransformSpace& Space);

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static FRotator GetRotation(AActor* TargetActor, const ETransformSpace& Space);



	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actor")
	static bool IsAttached(AActor* TargetActor);
};
