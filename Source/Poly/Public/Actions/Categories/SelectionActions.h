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
		this->Ids.Empty(0);
		for (int i = 0; i < InPolySelection.Num(); i++)
		{
			this->Ids.Add(InPolySelection[i]->TargetId);
		}
	}
	//SetSelectionAction->SelectorName = USelectorNames::Actors;
	//for (int i = 0; i < this->PolySelection.Num(); i++)
	//{
	//	SetSelectionAction->Ids.Add(this->PolySelection[i]->TargetId);
	//}


};
