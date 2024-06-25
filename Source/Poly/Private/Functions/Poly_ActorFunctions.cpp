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

//FVector UPoly_ActorFunctions::GetLocation(AActor* Actor, const ETransformSpace& Space)
//{
//	if (IsValid(Actor))
//	{
//		if (Space == ETransformSpace::TS_World)
//		{
//			USceneComponent* RootComp = Actor->GetRootComponent();
//			if (IsValid(RootComp))
//				return RootComp->GetComponentLocation();
//		}
//		return Actor->GetActorLocation();
//	}
//	return FVector::ZeroVector;
//}

FVector UPoly_ActorFunctions::GetLocation(AActor* Actor, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	if (IsValid(Actor))
	{
		if (Space == ETransformSpace::TS_World)
		{
			//if (Aggregation == EGizmoPivotAggregation::PA_Identity)
			return Actor->GetActorLocation();
			//else
			//{
			//	FVector Origin, Extents;
			//	FVector ActLoc = Actor->GetActorLocation();
			//	Actor->GetActorBounds(false, Origin, Extents);

			//	UE_LOG(LogTemp, Warning, TEXT("GetLocation (Actor): %s;"), *(Origin).ToString());
			//	return Origin;
			//}
		}
		else
		{
			//if (Aggregation == EGizmoPivotAggregation::PA_Identity)
			//	return Actor->GetActorLocation();
			//else
			//{
			//	FVector Origin, Extents;
			//	Actor->GetActorBounds(false, Origin, Extents);
			//	return Origin + Actor->GetActorLocation();
			//}
		}
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
	if (IsValid(Selection))
	{
		if (Selection->IsNotEmpty())
		{
			FGeometryScriptMeshSelection MeshSelection = Selection->GetMeshElementsSelection();
			UDynamicMesh* TargetMesh = Selection->GetSelectedMesh();

			FBox Bounds;
			bool bIsEmpty = true;
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
			Location + GetLocation(Actor, Space, Aggregation);
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
		if (!IsValid(Selections[i]))
			continue;
		Location += GetLocation(Selections[i], Space, Aggregation);
		Count++;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetLocation: %s;"), *Location.ToString());
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
		if (!IsValid(Selections[i]))
			continue;
		Location += GetLocation(Selections[i], Space, Aggregation);
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
	return FRotator();
}

FRotator UPoly_ActorFunctions::GetRotation(UPolySelection* Selection, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	if (IsValid(Selection))
		return GetRotation(Selection->GetSelectedActor(), Space, Aggregation);
	return FRotator::ZeroRotator;
}

FRotator UPoly_ActorFunctions::GetRotation(const TArray<UPolySelection*> Selections, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	return FRotator();
}

FRotator UPoly_ActorFunctions::GetRotation(const TArray<AActor*> Actors, const ETransformSpace& Space, const EGizmoPivotAggregation& Aggregation)
{
	return FRotator();
}


bool UPoly_ActorFunctions::IsAttached(AActor* Actor)
{
	if (IsValid(Actor))
	{
		return (IsValid(Actor->GetAttachParentActor()) || IsValid(Actor->GetParentActor()));
	}
	return false;
}


void UPoly_ActorFunctions::AddActorTransform(AActor* Actor, FVector InOffset, FQuat InRotation, FVector InScale, ETransformSpace Space)
{
	if (IsValid(Actor))
		if (Space == ETransformSpace::TS_Local)
		{
			Actor->AddActorLocalOffset(InOffset);
			Actor->AddActorLocalRotation(InRotation);
			Actor->SetActorRelativeScale3D(Actor->GetActorRelativeScale3D() + InScale);
		}
		else
		{
			Actor->AddActorWorldOffset(InOffset);
			Actor->AddActorWorldRotation(InRotation);
			Actor->SetActorScale3D(Actor->GetActorScale3D() + InScale);
		}
}

void UPoly_ActorFunctions::AddActorTransform(AActor* Actor, FTransform InTransform, ETransformSpace Space)
{
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
