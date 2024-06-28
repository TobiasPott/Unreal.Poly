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
	FVector Normal;
	FVector Tangent;
	int32 Count = 0;
	for (int i = 0; i < Indices.Num(); i++)
	{
		bool bIsValidTriangle = false;
		FVector Barycentric = FVector::ZeroVector;;
		FVector InterpNormal;
		FVector InterpTangent;
		FVector InterpBiTangent;
		UGeometryScriptLibrary_MeshQueryFunctions::GetInterpolatedTriangleNormalTangents(TargetMesh, Indices[i], Barycentric, bIsValidTriangle, InterpNormal, InterpTangent, InterpBiTangent);
		if (bIsValidTriangle)
		{
			Normal += InterpNormal;
			Tangent += InterpTangent;
			Count++;
		}
	}
	
	Normal /= ((float)Count);
	Tangent /= ((float)Count);

	OutNormal = Normal;
	OutTangent = Tangent;
	// ToDo: :CONTINUE (derive normal/orientation from mesh elements selection)
	return Count > 0;
}
