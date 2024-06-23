// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_MeshEditFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshTransformFunctions.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"



//void UPoly_MeshEditFunctions::AddMeshElementsTransform(AActor* Actor, FVector InOffset, FQuat InRotation, FVector InScale, ETransformSpace Space)
//{
//	if (IsValid(Actor))
//		if (Space == ETransformSpace::TS_Local)
//		{
//			Actor->AddActorLocalOffset(InOffset);
//			Actor->AddActorLocalRotation(InRotation);
//			Actor->SetActorRelativeScale3D(Actor->GetActorRelativeScale3D() + InScale);
//		}
//		else
//		{
//			Actor->AddActorWorldOffset(InOffset);
//			Actor->AddActorWorldRotation(InRotation);
//			Actor->SetActorScale3D(Actor->GetActorScale3D() + InScale);
//		}
//}

void UPoly_MeshEditFunctions::AddMeshElementsTransform(UDynamicMesh* TargetMesh, FGeometryScriptMeshSelection Selection, FTransform InTransform, ETransformSpace Space)
{
	if (IsValid(TargetMesh) && Selection.GetNumSelected() > 0)
		if (Space == ETransformSpace::TS_Local)
		{
			//Actor->AddActorLocalOffset(InTransform.GetLocation());
			//Actor->AddActorLocalRotation(InTransform.GetRotation());
			//Actor->SetActorRelativeScale3D(Actor->GetActorRelativeScale3D() + InTransform.GetScale3D());
		}
		else
		{
			UGeometryScriptLibrary_MeshTransformFunctions::TransformMeshSelection(TargetMesh, Selection, InTransform);
		}
}

//void UPoly_MeshEditFunctions::AddTransforms(const TArray<AActor*> Actors, FTransform InTransform, ETransformSpace Space)
//{
//	if (Space == ETransformSpace::TS_Local)
//	{
//		for (int i = 0; i < Actors.Num(); i++)
//		{
//			AActor* Selected = Actors[i];
//			Selected->AddActorLocalOffset(InTransform.GetLocation());
//			Selected->AddActorLocalRotation(InTransform.GetRotation());
//			Selected->SetActorRelativeScale3D(Selected->GetActorRelativeScale3D() + InTransform.GetScale3D());
//		}
//	}
//	else
//	{
//		for (int i = 0; i < Actors.Num(); i++)
//		{
//			AActor* Selected = Actors[i];
//			Selected->AddActorWorldOffset(InTransform.GetLocation());
//			Selected->AddActorWorldRotation(InTransform.GetRotation());
//			Selected->SetActorScale3D(Selected->GetActorScale3D() + InTransform.GetScale3D());
//		}
//	}
//}
