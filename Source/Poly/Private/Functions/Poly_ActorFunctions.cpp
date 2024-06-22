// Fill out your copyright notice in the Description page of Project Settings.

#include "Functions/Poly_ActorFunctions.h"
#include "Components/BaseDynamicMeshComponent.h"

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

FVector UPoly_ActorFunctions::GetLocation(AActor* Actor, const ETransformSpace& Space)
{
	if (IsValid(Actor))
	{
		if (Space == ETransformSpace::TS_World)
		{
			USceneComponent* RootComp = Actor->GetRootComponent();
			if (IsValid(RootComp))
				return RootComp->GetComponentLocation();
		}
		return Actor->GetActorLocation();
	}
	return FVector::ZeroVector;
}

FRotator UPoly_ActorFunctions::GetRotation(AActor* Actor, const ETransformSpace& Space)
{
	if (IsValid(Actor))
	{
		if (Space == ETransformSpace::TS_World)
		{
			USceneComponent* RootComp = Actor->GetRootComponent();
			if (IsValid(RootComp))
				return RootComp->GetComponentRotation();
		}
		return Actor->GetActorRotation();
	}
	return FRotator::ZeroRotator;
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
