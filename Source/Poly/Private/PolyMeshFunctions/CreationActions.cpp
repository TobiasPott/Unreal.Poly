// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyMeshFunctions/CreationActions.h"
#include "PolyMeshFunctions/PolyMesh_CreationFunctions.h"
#include "PolyMeshActor.h"
#include "Scene/SceneSubsystemFunctions.h"


bool UCreateBoxAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPolyMesh_CreationFunctions::CreateBoxActor(this, PolyMeshActor, this->Offset, this->Dimensions);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);

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

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);

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

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);

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

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);

	if (IsValid(PolyMeshActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(PolyMeshActor);
}
