// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDynamicMesh.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "Modeling/PolySelection.h"
#include "Modeling/PolyMeshSelection.h"
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

	/**
	* Poly Selection Functions
	*/
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByIdentifier(TArray<UPolySelection*>& Array, UIdentifierComponent* Identifier);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByIdentifiers(TArray<UPolySelection*>& Array, TArray<UIdentifierComponent*> Identifiers);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveById(TArray<UPolySelection*>& Array, int32 Id);
	UFUNCTION(BlueprintCallable, Category = "Poly|Selection")
	static int32 RemoveByIds(TArray<UPolySelection*>& Array, TArray<int32> Ids);


};
