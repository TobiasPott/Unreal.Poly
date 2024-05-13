// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/GizmoFunctionLibrary.h"

FVector UGizmoFunctionLibrary::GetLocation(const FGizmoPivot& Pivot)
{
	return Pivot.GetLocation();
}

FRotator UGizmoFunctionLibrary::GetOrientation(const FGizmoPivot& Pivot)
{
	return Pivot.GetOrientation();
}

ETransformSpace UGizmoFunctionLibrary::GetSpace(const FGizmoPivot& Pivot)
{
	return Pivot.GetSpace();
}

ETransformSpace UGizmoFunctionLibrary::GetInverseSpace(const FGizmoPivot& Pivot)
{
	return Pivot.GetInverseSpace();
}


bool UGizmoFunctionLibrary::IsLocalSpace(const FGizmoPivot& Pivot)
{
	return Pivot.IsLocalSpace();
}

bool UGizmoFunctionLibrary::IsWorldSpace(const FGizmoPivot& Pivot)
{
	return Pivot.IsWorldSpace();
}