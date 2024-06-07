// Fill out your copyright notice in the Description page of Project Settings.


#include "Modeling/PolySelection.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "Functions/Poly_ActorFunctions.h"


bool UPolySelection::Resolve_Implementation()
{
	if (IsValid(this->TargetIdentifier) && IsValid(this->TargetActor))
	{
		if (this->TargetId == this->TargetIdentifier->Id)
			return true;
	}
	if (UPoly_IdentifierFunctions::GetActorById(this->TargetId, TargetActor, TargetIdentifier))
	{
		return true;
	}
	return false;
}

bool UPolySelection::IsSelected(AActor* InActor) const
{
	return IsValid(this->TargetActor) ? this->TargetActor == InActor : false;
}

bool UPolySelection::IsSelected(UIdentifierComponent* InIdentifier) const
{
	return IsValid(this->TargetIdentifier) ? this->TargetIdentifier == InIdentifier : false;
}

bool UPolySelection::IsSelected(int32 InId) const
{
	return IsValid(this->TargetIdentifier) ? this->TargetIdentifier->Id == InId : false;
}
