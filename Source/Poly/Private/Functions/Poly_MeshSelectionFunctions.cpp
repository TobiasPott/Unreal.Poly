// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"

bool UPoly_MeshSelectionFunctions::GetSelectionCenterOfBounds(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter)
{
	if (Selection.GetNumSelected() == 0)
	{
		OutCenter = FVector::ZeroVector;
		return false;
	}
	else
	{
		FBox Bounds;
		bool bIsEmpty = false;
		UGeometryScriptLibrary_MeshSelectionQueryFunctions::GetMeshSelectionBoundingBox(TargetMesh, Selection, Bounds, bIsEmpty);
		if (!bIsEmpty)
		{
			OutCenter = Bounds.GetCenter();
			return true;
		}

		OutCenter = FVector::ZeroVector;
		return false;
	}
}

bool UPoly_MeshSelectionFunctions::GetSelectionCenterOfBoundsFromActor(AActor* TargetActor, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter)
{
	if (IsValid(TargetActor))
	{
		UDynamicMesh* TargetMesh;
		if (UPoly_ActorFunctions::GetDynamicMesh(TargetActor, TargetMesh)
			&& UPoly_MeshSelectionFunctions::GetSelectionCenterOfBounds(TargetMesh, Selection, OutCenter))
		{
			OutCenter = TargetActor->GetTransform().TransformPosition(OutCenter);
			return true;
		}
	}

	OutCenter = FVector::ZeroVector;
	return false;
}




/**
* Poly Selection Functions
*/


int32 UPoly_MeshSelectionFunctions::RemoveByActor(TArray<UPolySelection*>& Array, AActor* Actor)
{
	int32 NumRemoved = 0;
	for (int i = Array.Num() - 1; i > 0; i--)
	{
		UPolySelection* Selection = Array[i];
		if (Selection->IsSelectedActor(Actor))
		{
			Array.RemoveAt(i);
			NumRemoved++;
		}
	}
	return NumRemoved;
}

int32 UPoly_MeshSelectionFunctions::RemoveByActors(TArray<UPolySelection*>& Array, TArray<AActor*> Actors)
{
	int32 NumRemoved = 0;
	for (auto Item : Actors)
		NumRemoved += RemoveByActor(Array, Item);
	return NumRemoved;
}

int32 UPoly_MeshSelectionFunctions::RemoveByIdentifier(TArray<UPolySelection*>& Array, UIdentifierComponent* Identifier)
{
	int32 NumRemoved = 0;
	for (int i = Array.Num() - 1; i > 0; i--)
	{
		UPolySelection* Selection = Array[i];
		if (Selection->IsSelectedIdentifier(Identifier))
		{
			Array.RemoveAt(i);
			NumRemoved++;
		}
	}
	return NumRemoved;
}

int32 UPoly_MeshSelectionFunctions::RemoveByIdentifiers(TArray<UPolySelection*>& Array, TArray<UIdentifierComponent*> Identifiers)
{
	int32 NumRemoved = 0;
	for (auto Item : Identifiers)
		NumRemoved += RemoveByIdentifier(Array, Item);
	return NumRemoved;
}

int32 UPoly_MeshSelectionFunctions::RemoveById(TArray<UPolySelection*>& Array, int32 Id)
{
	int32 NumRemoved = 0;
	for (int i = Array.Num() - 1; i > 0; i--)
	{
		UPolySelection* Selection = Array[i];
		if (Selection->IsSelectedId(Id))
		{
			Array.RemoveAt(i);
			NumRemoved++;
		}
	}
	return NumRemoved;
}

int32 UPoly_MeshSelectionFunctions::RemoveByIds(TArray<UPolySelection*>& Array, TArray<int32> Ids)
{
	int32 NumRemoved = 0;
	for (auto Item : Ids)
		NumRemoved += RemoveById(Array, Item);
	return NumRemoved;
}
