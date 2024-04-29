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

bool UCreateSphereAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPolyMesh_CreationFunctions::CreateSphereActor(this, PolyMeshActor, this->Offset, this->Radius, this->StepX, this->StepY, this->StepZ);
	if (IsValid(PolyMeshActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(PolyMeshActor);
}

bool UCreateCylinderAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPolyMesh_CreationFunctions::CreateCylinderActor(this, PolyMeshActor, this->Offset, this->Radius, this->Height, this->RadialSteps, this->HeightSteps);
	if (IsValid(PolyMeshActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(PolyMeshActor);
}

bool UCreateConeAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPolyMesh_CreationFunctions::CreateConeActor(this, PolyMeshActor, this->Offset, this->BaseRadius, this->TopRadius, this->Height, this->RadialSteps, this->HeightSteps);
	if (IsValid(PolyMeshActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(PolyMeshActor);
}
