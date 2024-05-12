// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
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
