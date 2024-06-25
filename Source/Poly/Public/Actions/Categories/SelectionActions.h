// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "GeometryScript/MeshModelingFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "Selection/SelectorTypes.h"
#include "SelectionActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UClearSelectionAction : public UActionBase
{
	GENERATED_BODY()

protected: 
	// Ctor
	UClearSelectionAction(const FString& InDescription, const FString& InShortName) : UActionBase(InDescription, InShortName) {}
public:
	// Ctor
	UClearSelectionAction() : UActionBase("poly.ClearSelection", "Clear Selection") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;

public:
	bool Execute_Implementation(bool bEmitRecord) override;


	void SetupWith(FName InName)
	{
		this->SelectorName = InName;
	}
}; 

UCLASS(Blueprintable)
class POLY_API UClearElementsSelectionAction : public UClearSelectionAction
{
	GENERATED_BODY()

public:
	// Ctor
	UClearElementsSelectionAction() : UClearSelectionAction("poly.ClearElementsSelection", "Clear Elements Selection") { this->SelectorName = USelectorNames::Elements; }
};

UCLASS(Blueprintable)
class POLY_API USetSelectionAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	USetSelectionAction() : UActionBase("poly.SetSelection", "Set Selection") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TSet<int32> Ids;

public:
	bool Execute_Implementation(bool bEmitRecord) override;


	//UFUNCTION(BlueprintCallable, Category = "Selection")

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolySelectionType>
	void SetupWith(FName InName, const TArray<TPolySelectionType*> InPolySelection)
	{
		this->SelectorName = InName;
		int32 Num = InPolySelection.Num();
		this->Ids.Empty(Num);
		for (int i = 0; i < Num; i++)
		{
			TPolySelectionType* PolySelection = InPolySelection[i];
			if (IsValid(PolySelection))
			{
				this->Ids.Add(PolySelection->TargetId);
			}
		}
	}
};


UCLASS(Blueprintable)
class POLY_API USetElementsSelectionAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	USetElementsSelectionAction() : UActionBase("poly.SetElementsSelection", "Set Elements Selection") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TArray<int32> Ids;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TArray<FGeometryScriptMeshSelection> Selections;

public:
	bool Execute_Implementation(bool bEmitRecord) override;


	//UFUNCTION(BlueprintCallable, Category = "Selection")

	/**
	 * Documentation missing (@tpott).
	 * @param	Array
	 * @param	Actor
	 */
	template<class TPolyMeshSelectionType>
	void SetupWith(FName InName, const TArray<TPolyMeshSelectionType*> InPolySelection)
	{
		this->SelectorName = InName;
		const int32 Num = InPolySelection.Num();
		this->Ids.Empty(Num);
		this->Selections.Empty(Num);
		for (int i = 0; i < Num; i++)
		{
			TPolyMeshSelectionType* PolySelection = InPolySelection[i];
			if (IsValid(PolySelection))
			{
				this->Ids.Add(PolySelection->TargetId);
				this->Selections.Add(PolySelection->Selection);
			}
		}
	}
};