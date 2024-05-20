// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "Modeling/PolySelection.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "Poly_MeshSelectionFunctions.generated.h"

/**
 *
 */
UCLASS()
class POLY_API UPoly_MeshSelectionFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Geometry Script|Selection")
	static bool GetSelectionCenterOfBounds(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter);
	UFUNCTION(BlueprintCallable, Category = "Geometry Script|Selection")
	static bool GetSelectionCenterOfBoundsFromActor(AActor* TargetActor, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter);



	// ToDo: @tpott: Move template functions to UPolySelection type?! (Conideration requied)

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	static int32 AddByActorT(TArray<TPolySelectionType*>& Array, AActor* Actor)
	{
		if (!IsValid(Actor))
			return INDEX_NONE;
		int32 Index = Array.IndexOfByPredicate([Actor](TPolySelectionType* Item) { return Item->IsSelectedActor(Actor); });
		if (Index != INDEX_NONE)
			return Index;

		//ToDo: @tpott:This is incomplete!: Add creation of UPolySelection from the given actor using the queried Identifier
		//				Consider adding class param to control which UPolySelection to create.
		UIdentifierComponent* IdComp;
		int32 Id;
		UPoly_IdentifierFunctions::GetActorId(Actor, Id, IdComp, true);

		UClass* ReturnType = TPolySelectionType::StaticClass();
		TPolySelectionType* Result = NewObject<TPolySelectionType>(Actor);
		Result->TargetId = Id;
		if (Result->Resolve())
		{
			Index = Array.Add(Result);
			return Index;
		}
		return INDEX_NONE;
	}

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actors
	 */
	template<class TPolySelectionType>
	static int32 AddByActorsT(TArray<TPolySelectionType*>& Array, TArray<AActor*> Actors)
	{
		int32 NumAdded = 0;
		for (auto Item : Actors)
		{
			if (UPoly_MeshSelectionFunctions::AddByActorT<TPolySelectionType>(Array, Item) != INDEX_NONE)
				NumAdded++;
		}
		return NumAdded;
	}

	/**
	* Poly Selection Functions
	*/


	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByActor(TArray<UPolySelection*>& Array, AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByActors(TArray<UPolySelection*>& Array, TArray<AActor*> Actors);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByIdentifier(TArray<UPolySelection*>& Array, UIdentifierComponent* Identifier);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByIdentifiers(TArray<UPolySelection*>& Array, TArray<UIdentifierComponent*> Identifiers);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveById(TArray<UPolySelection*>& Array, int32 Id);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByIds(TArray<UPolySelection*>& Array, TArray<int32> Ids);


};
