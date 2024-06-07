// Fill out your copyright notice in the Description page of Project Settings.


#include "Modeling/PolyMeshSelection.h"
#include "Functions/Poly_IdentifierFunctions.h"
#include "Functions/Poly_ActorFunctions.h"


bool UPolyMeshSelection::Resolve_Implementation()
{
	// culprit for recursive stack overflow crash?
	if (Super::Resolve_Implementation())
	{
		// try get dynamic mesh from actor (assumes only one component exists)s
		if (UPoly_ActorFunctions::GetDynamicMesh(this->TargetActor, this->TargetMesh))
			return true;
	}
	return false;
}
