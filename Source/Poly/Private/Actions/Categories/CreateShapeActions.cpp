// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/CreateShapeActions.h"
#include "Actions/ActionMacros.h"
#include "Functions/Poly_CreationFunctions.h"
#include "Scene/SceneSubsystemFunctions.h"
#include "PolyMeshActor.h"


bool UCreateBoxAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateBoxActor(this, PolyMeshActor, this->Offset, this->Dimensions);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(PolyMeshActor)
}

bool UCreateSphereAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateSphereActor(this, PolyMeshActor, this->Offset, this->Radius, this->StepX, this->StepY, this->StepZ);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(PolyMeshActor)
}

bool UCreateCylinderAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateCylinderActor(this, PolyMeshActor, this->Offset, this->Radius, this->Height, this->RadialSteps, this->HeightSteps);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(PolyMeshActor)
}

bool UCreateConeAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateConeActor(this, PolyMeshActor, this->Offset, this->BaseRadius, this->TopRadius, this->Height, this->RadialSteps, this->HeightSteps);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(PolyMeshActor)
}
