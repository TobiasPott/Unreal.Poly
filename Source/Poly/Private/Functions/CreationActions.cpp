// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/CreationActions.h"
#include "Functions/Poly_CreationFunctions.h"
#include "PolyMeshActor.h"
#include "Scene/SceneSubsystemFunctions.h"


#define RETURN_ActionSubmit(Actor)	\
		{							\
			if (IsValid(Actor))		\
				this->Submit();		\
			else					\
				this->Discard();	\
			return IsValid(Actor);	\
		}

bool UCreateBoxAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateBoxActor(this, PolyMeshActor, this->Offset, this->Dimensions);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ActionSubmit(PolyMeshActor)
}

bool UCreateSphereAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateSphereActor(this, PolyMeshActor, this->Offset, this->Radius, this->StepX, this->StepY, this->StepZ);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ActionSubmit(PolyMeshActor)
}

bool UCreateCylinderAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateCylinderActor(this, PolyMeshActor, this->Offset, this->Radius, this->Height, this->RadialSteps, this->HeightSteps);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ActionSubmit(PolyMeshActor)
}

bool UCreateConeAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateConeActor(this, PolyMeshActor, this->Offset, this->BaseRadius, this->TopRadius, this->Height, this->RadialSteps, this->HeightSteps);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);
	RETURN_ActionSubmit(PolyMeshActor)
}

// ToDo: @tpott: Convert code to template function to reduce code repetition
bool UCreateDirectionalLightAction::Execute_Implementation(bool bEmitRecord)
{
	ADirectionalLight* LightActor;
	UPoly_CreationFunctions::CreateDirectioalLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);
	RETURN_ActionSubmit(LightActor)
}

bool UCreateSpotLightAction::Execute_Implementation(bool bEmitRecord)
{
	ASpotLight* LightActor;
	UPoly_CreationFunctions::CreateSpotLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);
	RETURN_ActionSubmit(LightActor)
}

bool UCreatePointLightAction::Execute_Implementation(bool bEmitRecord)
{
	APointLight* LightActor;
	UPoly_CreationFunctions::CreatePointLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);
	RETURN_ActionSubmit(LightActor)
}
