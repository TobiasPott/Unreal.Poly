// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "IdentifierComponent.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "PolySelection.generated.h"


/**
 *
 */
UCLASS(BlueprintType)
class POLY_API UPolySelection : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UIdentifierComponent* TargetIdentifier = nullptr;
	UPROPERTY()
	AActor* TargetActor = nullptr;


public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	int32 TargetId = -1;


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selection")
	bool Resolve();
	virtual bool Resolve_Implementation();



	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	AActor* GetSelectedActor() const { return TargetActor; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	UIdentifierComponent* GetSelectedIdentifier() const { return TargetIdentifier; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	int32 GetSelectedId() const { return TargetId; };


	bool IsSelected(AActor* InActor) const;
	bool IsSelected(UIdentifierComponent* InIdentifier) const;
	bool IsSelected(int32 InId) const;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	bool IsSelectedActor(AActor* InActor) const { return IsSelected(InActor); };
	UFUNCTION(BlueprintCallable, Category = "Selection")
	bool IsSelectedIdentifier(UIdentifierComponent* InIdentifier) const { return IsSelected(InIdentifier); };
	UFUNCTION(BlueprintCallable, Category = "Selection")
	bool IsSelectedId(int32 InId) const { return IsSelected(InId); };


public:
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
			if (UPolySelection::AddByActorT<TPolySelectionType>(Array, Item) != INDEX_NONE)
				NumAdded++;
		}
		return NumAdded;
	}

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Id
	 */
	template<class TPolySelectionType>
	static int32 AddByIdT(TArray<TPolySelectionType*>& Array, int32 Id)
	{
		int32 Index = INDEX_NONE;
		UIdentifierComponent* IdComp;
		AActor* IdActor;
		UPoly_IdentifierFunctions::GetActorById(Id, IdActor, IdComp);
		
		// ToDo: @tpott: debug for valid actor and set selection members from 'GEtActorById' results
		if (IsValid(IdActor))
		{
			TPolySelectionType* Result = NewObject<TPolySelectionType>(IdActor);
			Result->TargetId = Id;
			if (Result->Resolve())
			{
				Index = Array.Add(Result);
				return Index;
			}
		}
		return INDEX_NONE;
	}
	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actors
	 */
	template<class TPolySelectionType>
	static int32 RemoveByActorsT(TArray<TPolySelectionType*>& Array, TArray<AActor*> Actors)
	{
		int32 NumRemoved = 0;
		for (auto Item : Actors)
			NumRemoved += RemoveByT(Array, Item);
		return NumRemoved;
	}

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	SelectedBy
	 */
	template<class TPolySelectionType, class TSelectedByType>
	static int32 RemoveByT(TArray<TPolySelectionType*>& Array, TSelectedByType SelectedBy)
	{
		int32 NumRemoved = 0;
		for (int i = Array.Num() - 1; i >= 0; i--)
		{
			TPolySelectionType* Selection = Array[i];
			if (Selection->IsSelected(SelectedBy))
			{
				Array.RemoveAt(i);
				NumRemoved++;
			}
		}
		return NumRemoved;
	}


	///**
	// * Documentation missing (@tpott).
	// * @param	Array
	// * @param	OutIds
	// * @param	bReplace
	// */
	//template<class TPolySelectionType>
	//static void AddIdsTo(const TArray<TPolySelectionType*>& Array, TSet<int32> OutIds, bool bReplace = true)
	//{
	//	if (bReplace)
	//		OutIds.Empty(Array.Num());
	//	for (TPolySelectionType* Item : Array)
	//	{
	//		OutIds.Add(Item->TargetId);
	//	}
	//}


};
