// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"


/**
* Poly Selection Functions
*/
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
