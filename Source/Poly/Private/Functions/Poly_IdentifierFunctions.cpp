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
