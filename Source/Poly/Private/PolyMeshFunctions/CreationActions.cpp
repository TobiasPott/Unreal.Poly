// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyMeshFunctions/CreationActions.h"
#include "PolyMeshFunctions/PolyMesh_CreationFunctions.h"
#include "PolyMeshActor.h"


bool UCreateBoxAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPolyMesh_CreationFunctions::CreateBoxActor(this, PolyMeshActor, this->Offset, this->Dimensions);
	if (IsValid(PolyMeshActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(PolyMeshActor);
}
