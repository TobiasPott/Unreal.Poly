// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_MeshSelectionFunctions.h"
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
