// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/GizmoFunctionLibrary.h"

inline FVector UGizmoFunctionLibrary::GetLocation(const FGizmoPivot& Pivot)
{
	return Pivot.GetLocation();
}

inline FRotator UGizmoFunctionLibrary::GetOrientation(const FGizmoPivot& Pivot)
{
	return Pivot.GetOrientation();
}

inline ETransformSpace UGizmoFunctionLibrary::GetSpace(const FGizmoPivot& Pivot)
{
	return Pivot.GetSpace();
}

ETransformSpace UGizmoFunctionLibrary::GetInverseSpace(const FGizmoPivot& Pivot)
{
	return Pivot.GetInverseSpace();
}


inline bool UGizmoFunctionLibrary::IsLocalSpace(const FGizmoPivot& Pivot)
{
	return Pivot.IsLocalSpace();
}

inline bool UGizmoFunctionLibrary::IsWorldSpace(const FGizmoPivot& Pivot)
{
	return Pivot.IsWorldSpace();
}