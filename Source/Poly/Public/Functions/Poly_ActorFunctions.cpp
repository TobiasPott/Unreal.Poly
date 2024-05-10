// Fill out your copyright notice in the Description page of Project Settings.

#include "Poly_ActorFunctions.h"

FVector UPoly_ActorFunctions::GetLocation(AActor* TargetActor, const ETransformSpace& Space)
{
	if (IsValid(TargetActor))
	{
		if (Space == ETransformSpace::TS_World)
		{
			USceneComponent* RootComp = TargetActor->GetRootComponent();
			if (IsValid(RootComp))
				return RootComp->GetComponentLocation();
		}
		return TargetActor->GetActorLocation();
	}
	return FVector::ZeroVector;
}

FRotator UPoly_ActorFunctions::GetRotation(AActor* TargetActor, const ETransformSpace& Space)
{
	if (IsValid(TargetActor))
	{
		if (Space == ETransformSpace::TS_World)
		{
			USceneComponent* RootComp = TargetActor->GetRootComponent();
			if (IsValid(RootComp))
				return RootComp->GetComponentRotation();
		}
		return TargetActor->GetActorRotation();
	}
	return FRotator::ZeroRotator;
}
