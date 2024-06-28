// Fill out your copyright notice in the Description page of Project Settings.

#include "Functions/Poly_ActorFunctions.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "GeometryScript/MeshSelectionQueryFunctions.h"

bool UPoly_ActorFunctions::GetDynamicMesh(AActor* Actor, UDynamicMesh*& Mesh)
{
	if (IsValid(Actor))
	{
		UBaseDynamicMeshComponent* DMC = Actor->GetComponentByClass<UBaseDynamicMeshComponent>();
		if (IsValid(DMC))
		{
			Mesh = DMC->GetDynamicMesh();
			return true;
		}
	}

	Mesh = nullptr;
	return false;
}

FVector UPoly_ActorFunctions::GetLocation(AActor* Actor, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	if (IsValid(Actor))
	{
		if (Space == ETransformSpace::TS_World)
			return Actor->GetActorLocation();
		else
			return Actor->GetRootComponent()->GetRelativeLocation();
	}
	return FVector::ZeroVector;
}

FVector UPoly_ActorFunctions::GetLocation(UPolySelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	if (IsValid(Selection))
		return GetLocation(Selection->GetSelectedActor(), Space, Aggregation);
	return FVector::ZeroVector;
}

FVector UPoly_ActorFunctions::GetLocation(UPolyMeshSelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FBox Bounds;
	bool bIsEmpty = true;
	if (IsValid(Selection))
	{
		if (Selection->IsNotEmpty())
		{
			FGeometryScriptMeshSelection MeshSelection = Selection->GetMeshElementsSelection();
			UDynamicMesh* TargetMesh = Selection->GetSelectedMesh();

			UGeometryScriptLibrary_MeshSelectionQueryFunctions::GetMeshSelectionBoundingBox(TargetMesh, MeshSelection, Bounds, bIsEmpty);

			FTransform ActorTransform = Selection->GetSelectedActor()->GetActorTransform();
			return ActorTransform.TransformPosition(Bounds.GetCenter());
		}
		return GetLocation(Selection->GetSelectedActor(), Space, Aggregation);
	}
	return FVector::ZeroVector;
}

FVector UPoly_ActorFunctions::GetLocation(const TArray<AActor*> Actors, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FVector Location = FVector::ZeroVector;
	int32 Count = 0;
	for (int i = 0; i < Actors.Num(); i++)
	{
		AActor* Actor = Actors[i];
		if (IsValid(Actor))
		{
			Location += GetLocation(Actor, Space, Aggregation);
			Count++;
		}
	}
	if (Count == 0)
		return FVector::ZeroVector;
	return Location / Count;
}

FVector UPoly_ActorFunctions::GetLocation(const TArray<UPolySelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FVector Location = FVector::ZeroVector;
	int32 Count = 0;
	for (int i = 0; i < Selections.Num(); i++)
	{
		UPolySelection* Selection = Selections[i];
		if (!IsValid(Selection))
			continue;
		Location += GetLocation(Selection, Space, Aggregation);
		Count++;
	}

	if (Count == 0)
		return FVector::ZeroVector;
	return Location / Count;
}

FVector UPoly_ActorFunctions::GetLocation(const TArray<UPolyMeshSelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FVector Location = FVector::ZeroVector;
	int32 Count = 0;
	for (int i = 0; i < Selections.Num(); i++)
	{
		UPolyMeshSelection* Selection = Selections[i];
		if (!IsValid(Selection))
			continue;
		Location += GetLocation(Selection, Space, Aggregation);
		Count++;
	}
	if (Count == 0)
		return FVector::ZeroVector;
	return Location / Count;
}

//
//FRotator UPoly_ActorFunctions::GetRotation(AActor* Actor, const ETransformSpace& Space)
//{
//	if (IsValid(Actor))
//	{
//		if (Space == ETransformSpace::TS_World)
//		{
//			USceneComponent* RootComp = Actor->GetRootComponent();
//			if (IsValid(RootComp))
//				return RootComp->GetComponentRotation();
//		}
//		return Actor->GetActorRotation();
//	}
//	return FRotator::ZeroRotator;
//}

FRotator UPoly_ActorFunctions::GetRotation(AActor* Actor, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	if (IsValid(Actor))
	{
		if (Space == ETransformSpace::TS_World)
			return Actor->GetActorRotation();
		else
			return Actor->GetRootComponent()->GetRelativeRotation();
	}
	return FRotator::ZeroRotator;
}

FRotator UPoly_ActorFunctions::GetRotation(UPolySelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	if (IsValid(Selection))
		return GetRotation(Selection->GetSelectedActor(), Space, Aggregation);
	return FRotator::ZeroRotator;
}

FRotator UPoly_ActorFunctions::GetRotation(UPolyMeshSelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	UE_LOG(LogTemp, Warning, TEXT("GetRotation(UPolyMeshSelection...) is not implemented"));
	return FRotator();
}

FRotator UPoly_ActorFunctions::GetRotation(const TArray<AActor*> Actors, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FRotator Location = FRotator::ZeroRotator;
	int32 Count = 0;
	for (int i = 0; i < Actors.Num(); i++)
	{
		AActor* Actor = Actors[i];
		if (IsValid(Actor))
		{
			Location += GetRotation(Actor, Space, Aggregation);
			Count++;
		}
	}
	if (Count == 0)
		return FRotator::ZeroRotator;
	return Location * (1.0f / Count);
}

FRotator UPoly_ActorFunctions::GetRotation(const TArray<UPolySelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FRotator Location = FRotator::ZeroRotator;
	int32 Count = 0;
	for (int i = 0; i < Selections.Num(); i++)
	{
		UPolySelection* Selection = Selections[i];
		if (IsValid(Selection))
		{
			Location += GetRotation(Selection, Space, Aggregation);
			Count++;
		}
	}
	if (Count == 0)
		return FRotator::ZeroRotator;
	return Location * (1.0f / Count);
}

FRotator UPoly_ActorFunctions::GetRotation(const TArray<UPolyMeshSelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	FRotator Location = FRotator::ZeroRotator;
	int32 Count = 0;
	for (int i = 0; i < Selections.Num(); i++)
	{
		UPolyMeshSelection* Selection = Selections[i];
		if (IsValid(Selection))
		{
			Location += GetRotation(Selection, Space, Aggregation);
			Count++;
		}
	}
	if (Count == 0)
		return FRotator::ZeroRotator;
	return Location * (1.0f / Count);
}



bool UPoly_ActorFunctions::IsAttached(AActor* Actor)
{
	if (IsValid(Actor))
	{
		return (IsValid(Actor->GetAttachParentActor()) || IsValid(Actor->GetParentActor()));
	}
	return false;
}

//
//void UPoly_ActorFunctions::AddActorTransform(AActor* Actor, FVector InOffset, FQuat InRotation, FVector InScale, ETransformSpace Space)
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

void UPoly_ActorFunctions::AddActorTransform(AActor* Actor, FTransform InTransform, ETransformSpace Space)
{
	// ToDo: @tpott: (AddActorTransform) Add transform around origin/pivot to this function (or add overload)
	if (IsValid(Actor))
		if (Space == ETransformSpace::TS_Local)
		{
			Actor->AddActorLocalOffset(InTransform.GetLocation());
			Actor->AddActorLocalRotation(InTransform.GetRotation());
			Actor->SetActorRelativeScale3D(Actor->GetActorRelativeScale3D() + InTransform.GetScale3D());
		}
		else
		{
			Actor->AddActorWorldOffset(InTransform.GetLocation());
			Actor->AddActorWorldRotation(InTransform.GetRotation());
			Actor->SetActorScale3D(Actor->GetActorScale3D() + InTransform.GetScale3D());
		}
}

void UPoly_ActorFunctions::AddTransforms(const TArray<AActor*> Actors, FTransform InTransform, ETransformSpace Space)
{
	if (Space == ETransformSpace::TS_Local)
	{
		for (int i = 0; i < Actors.Num(); i++)
		{
			AActor* Selected = Actors[i];
			Selected->AddActorLocalOffset(InTransform.GetLocation());
			Selected->AddActorLocalRotation(InTransform.GetRotation());
			Selected->SetActorRelativeScale3D(Selected->GetActorRelativeScale3D() + InTransform.GetScale3D());
		}
	}
	else
	{
		for (int i = 0; i < Actors.Num(); i++)
		{
			AActor* Selected = Actors[i];
			Selected->AddActorWorldOffset(InTransform.GetLocation());
			Selected->AddActorWorldRotation(InTransform.GetRotation());
			Selected->SetActorScale3D(Selected->GetActorScale3D() + InTransform.GetScale3D());
		}
	}
}
