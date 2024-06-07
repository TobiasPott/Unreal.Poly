// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/CreateLightActions.h"
#include "Actions/ActionMacros.h"
#include "Functions/Poly_CreationFunctions.h"
#include "Scene/SceneSubsystemFunctions.h"
#include "PolyMeshActor.h"

bool UCreateDirectionalLightAction::Execute_Implementation(bool bEmitRecord)
{
	ADirectionalLight* LightActor;
	UPoly_CreationFunctions::CreateDirectioalLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(LightActor)
}

bool UCreateSpotLightAction::Execute_Implementation(bool bEmitRecord)
{
	ASpotLight* LightActor;
	UPoly_CreationFunctions::CreateSpotLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(LightActor)
}

bool UCreatePointLightAction::Execute_Implementation(bool bEmitRecord)
{
	APointLight* LightActor;
	UPoly_CreationFunctions::CreatePointLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);
	RETURN_ACTIONSUBMIT_ISVALIDACTOR(LightActor)
}
