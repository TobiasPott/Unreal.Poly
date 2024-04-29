// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyMeshFunctions/PolyMesh_CreationFunctions.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "GeometryScript/MeshComparisonFunctions.h"
#include "GeometryScript/MeshDecompositionFunctions.h"
#include "GeometryScript/MeshMaterialFunctions.h"
#include "GeometryScript/MeshModelingFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/MeshRepairFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshTransformFunctions.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "Components/BaseDynamicMeshComponent.h"


void UPolyMesh_CreationFunctions::CreateBoxActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset, const FVector InDimensions)
{
	OutPolyActor = WorldContext->GetWorld()->SpawnActor<APolyMeshActor>();
	UDynamicMeshComponent* DMC = OutPolyActor->GetDynamicMeshComponent();
	UDynamicMesh* TargetMesh = DMC->GetDynamicMesh();

	const FGeometryScriptPrimitiveOptions Options = { EGeometryScriptPrimitivePolygroupMode::SingleGroup, false, EGeometryScriptPrimitiveUVMode::Uniform };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(TargetMesh, Options, FTransform::Identity, InDimensions.X, InDimensions.Y, InDimensions.Z, 0, 0, 0, EGeometryScriptPrimitiveOriginMode::Center);

	if (!InOffset.Equals(FVector::ZeroVector))
	{
		UGeometryScriptLibrary_MeshTransformFunctions::TranslateMesh(TargetMesh, InOffset);
	}
}

void UPolyMesh_CreationFunctions::CreateSphereActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset, const float Radius, int32 StepsX, int32 StepsY, int32 StepsZ)
{
	OutPolyActor = WorldContext->GetWorld()->SpawnActor<APolyMeshActor>();
	UDynamicMeshComponent* DMC = OutPolyActor->GetDynamicMeshComponent();
	UDynamicMesh* TargetMesh = DMC->GetDynamicMesh();

	const FGeometryScriptPrimitiveOptions Options = { EGeometryScriptPrimitivePolygroupMode::SingleGroup, false, EGeometryScriptPrimitiveUVMode::Uniform };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendSphereBox(TargetMesh, Options, FTransform::Identity, Radius, StepsX, StepsY, StepsZ, EGeometryScriptPrimitiveOriginMode::Center);
	if (!InOffset.Equals(FVector::ZeroVector))
	{
		UGeometryScriptLibrary_MeshTransformFunctions::TranslateMesh(TargetMesh, InOffset);
	}
}

void UPolyMesh_CreationFunctions::CreateCylinderActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset, const float Radius, const float Height, const int32 RadialSteps, const int32 HeightSteps)
{
	OutPolyActor = WorldContext->GetWorld()->SpawnActor<APolyMeshActor>();
	UDynamicMeshComponent* DMC = OutPolyActor->GetDynamicMeshComponent();
	UDynamicMesh* TargetMesh = DMC->GetDynamicMesh();

	const FGeometryScriptPrimitiveOptions Options = { EGeometryScriptPrimitivePolygroupMode::SingleGroup, false, EGeometryScriptPrimitiveUVMode::Uniform };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCylinder(TargetMesh, Options, FTransform::Identity, Radius, Height, RadialSteps, HeightSteps, true, EGeometryScriptPrimitiveOriginMode::Center);
	if (!InOffset.Equals(FVector::ZeroVector))
	{
		UGeometryScriptLibrary_MeshTransformFunctions::TranslateMesh(TargetMesh, InOffset);
	}
}

void UPolyMesh_CreationFunctions::CreateConeActor(const UObject* WorldContext, APolyMeshActor*& OutPolyActor, const FVector InOffset, const float BaseRadius, const float TopRadius, const float Height, const int32 RadialSteps, const int32 HeightSteps)
{
	OutPolyActor = WorldContext->GetWorld()->SpawnActor<APolyMeshActor>();
	UDynamicMeshComponent* DMC = OutPolyActor->GetDynamicMeshComponent();
	UDynamicMesh* TargetMesh = DMC->GetDynamicMesh();

	const FGeometryScriptPrimitiveOptions Options = { EGeometryScriptPrimitivePolygroupMode::SingleGroup, false, EGeometryScriptPrimitiveUVMode::Uniform };
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(TargetMesh, Options, FTransform::Identity, BaseRadius, TopRadius, Height, RadialSteps, HeightSteps, true, EGeometryScriptPrimitiveOriginMode::Center);
	if (!InOffset.Equals(FVector::ZeroVector))
	{
		UGeometryScriptLibrary_MeshTransformFunctions::TranslateMesh(TargetMesh, InOffset);
	}
}
