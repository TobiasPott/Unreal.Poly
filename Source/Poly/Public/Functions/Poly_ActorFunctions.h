// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Gizmos/GizmoTypes.h"
#include "Modeling/PolySelection.h"
#include "Modeling/PolyMeshSelection.h"
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

	static FVector GetLocation(AActor* Actor, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FVector GetLocation(UPolySelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FVector GetLocation(UPolyMeshSelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FVector GetLocation(const TArray<AActor*> Actors, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FVector GetLocation(const TArray<UPolySelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FVector GetLocation(const TArray<UPolyMeshSelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	
	static FRotator GetRotation(AActor* Actor, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FRotator GetRotation(UPolySelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FRotator GetRotation(UPolyMeshSelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FRotator GetRotation(const TArray<AActor*> Actors, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FRotator GetRotation(const TArray<UPolySelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);
	static FRotator GetRotation(const TArray<UPolyMeshSelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation);



	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actor")
	static bool IsAttached(AActor* TargetActor);




	//static void AddActorTransform(AActor* Actor, FVector InOffset, FQuat InRotation, FVector InScale, ETransformSpace Space = ETransformSpace::TS_World);
	static void AddActorTransform(AActor* Actor, FTransform InTransform, ETransformSpace Space = ETransformSpace::TS_World);
	static void AddTransforms(const TArray<AActor*> Actors, FTransform InTransform, ETransformSpace Space = ETransformSpace::TS_World);

};
