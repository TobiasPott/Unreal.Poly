// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_IdentifierFunctions.h"

bool UPoly_IdentifierFunctions::GetActorById(const int32 IdentifierId, AActor*& OutActor, UIdentifierComponent*& OutComponent)
{
	// Find the first Actor that matches the predicate
	OutComponent = *UIdentifierComponent::AllComponents.FindByPredicate([IdentifierId](UIdentifierComponent*& InItem) { return InItem->Id == IdentifierId; });
	if (IsValid(OutComponent))
		OutActor = OutComponent->GetOwner();
	return IsValid(OutComponent);
}

bool UPoly_IdentifierFunctions::GetActorId(AActor* Actor, int32& OutId, UIdentifierComponent*& OutComponent, const bool bAdd)
{
	
	if (GetActorIdentifier(Actor, OutComponent, bAdd))
	{
		OutId = OutComponent->Id;
		return true;
	}

	OutId = -1;
	OutComponent = nullptr;
	return false;
}

bool UPoly_IdentifierFunctions::GetActorIdentifier(AActor* Actor, UIdentifierComponent*& OutComponent, const bool bAdd)
{
	OutComponent = Actor->GetComponentByClass<UIdentifierComponent>();
	if (IsValid(OutComponent))
	{
		return true;
	}
	else if (bAdd)
	{
		// Create a new billboard component
		OutComponent = Actor->CreateDefaultSubobject<UIdentifierComponent>(TEXT("IdentifierComponent"));
		OutComponent->SetAutoIdentifier(Actor->GetActorNameOrLabel());
		return true;
	}
	
	OutComponent = nullptr;
	return false;
}

bool UPoly_IdentifierFunctions::GetActorsIds(TArray<AActor*> Actors, TArray<UIdentifierComponent*>& OutComponents, const bool bAdd)
{
	OutComponents.Empty();
	for (int i = 0; i < Actors.Num(); i++)
	{
		AActor* Actor = Actors[i];
		if (IsValid(Actor))
		{
			int32 Id;
			UIdentifierComponent* IdComp;
			if(GetActorId(Actor, Id, IdComp, bAdd))
			{
				OutComponents.AddUnique(IdComp);
			}
		}
	}
	return OutComponents.Num() > 0;
}
