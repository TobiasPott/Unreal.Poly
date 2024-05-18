// Fill out your copyright notice in the Description page of Project Settings.


#include "Modeling/PolySelection.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "Functions/Poly_ActorFunctions.h"


bool UPolySelection::Resolve()
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

bool UPolyMeshSelection::Resolve()
{
	if (Super::Resolve())
	{
		// try get dynamic mesh from actor (assumes only one component exists)s
		if (UPoly_ActorFunctions::GetDynamicMesh(this->TargetActor, this->TargetMesh))
			return true;
	}
	return false;
}
