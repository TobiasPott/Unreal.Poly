// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_MeshAnalysisFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/ListUtilityFunctions.h"


bool UPoly_MeshAnalysisFunctions::GetCenterOfSelectionBounds(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutCenter)
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
		// ToDo: @tpott: (GetCenterOfSelectionBounds) Implement function variant for precise center instead of bounds
		//				Convert selection to vertices and iterate over and gather their location
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

bool UPoly_MeshAnalysisFunctions::GetSelectionMormal(UDynamicMesh* TargetMesh, const FGeometryScriptMeshSelection& Selection, FVector& OutNormal, FVector& OutTangent)
{
	TArray<int32> Indices;
	EGeometryScriptMeshSelectionType Type = Selection.GetSelectionType();
	if (Type == EGeometryScriptMeshSelectionType::Polygroups)
		Type = EGeometryScriptMeshSelectionType::Triangles;
	UGeometryScriptLibrary_MeshSelectionFunctions::ConvertMeshSelectionToIndexArray(TargetMesh, Selection, Indices, Type);
	FVector Normal = FVector::ZeroVector;
	FVector Tangent = FVector::ZeroVector;
	int32 Count = 0;
	for (int i = 0; i < Indices.Num(); i++)
	{
		bool bIsValidTriangle = false;
		FVector InterpNormal = {};
		InterpNormal = UGeometryScriptLibrary_MeshQueryFunctions::GetTriangleFaceNormal(TargetMesh, Indices[i], bIsValidTriangle);
		if (bIsValidTriangle)
		{
			Normal += InterpNormal;
			//UE_LOG(LogTemp, Warning, TEXT("%d : %s (%s)"), i, *Normal.ToString(), *N1.ToString())
			Count++;
		}
	}

	Normal /= ((float)Count);
	//Tangent /= ((float)Count);

	OutNormal = Normal.GetUnsafeNormal();
	// ToDo: @tpott: (GetSelectionMormal) Reimplement tangent output
	OutTangent = FVector::UpVector;
	return Count > 0;
}
