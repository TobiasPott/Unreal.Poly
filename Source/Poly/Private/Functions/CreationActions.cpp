// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/CreationActions.h"
#include "Functions/CreationFunctions.h"
#include "PolyMeshActor.h"
#include "Scene/SceneSubsystemFunctions.h"


bool UCreateBoxAction::Execute_Implementation(bool bEmitRecord)
{
	APolyMeshActor* PolyMeshActor;
	UPoly_CreationFunctions::CreateBoxActor(this, PolyMeshActor, this->Offset, this->Dimensions);

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
	UPoly_CreationFunctions::CreateSphereActor(this, PolyMeshActor, this->Offset, this->Radius, this->StepX, this->StepY, this->StepZ);

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
	UPoly_CreationFunctions::CreateCylinderActor(this, PolyMeshActor, this->Offset, this->Radius, this->Height, this->RadialSteps, this->HeightSteps);

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
	UPoly_CreationFunctions::CreateConeActor(this, PolyMeshActor, this->Offset, this->BaseRadius, this->TopRadius, this->Height, this->RadialSteps, this->HeightSteps);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, PolyMeshActor);

	if (IsValid(PolyMeshActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(PolyMeshActor);
}

// ToDo: @tpott: Convert code to template function to reduce code repetition
bool UCreateDirectionalLightAction::Execute_Implementation(bool bEmitRecord)
{
	ADirectionalLight* LightActor;
	UPoly_CreationFunctions::CreateDirectioalLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);

	if (IsValid(LightActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(LightActor);
}

bool UCreateSpotLightAction::Execute_Implementation(bool bEmitRecord)
{
	ASpotLight* LightActor;
	UPoly_CreationFunctions::CreateSpotLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);

	if (IsValid(LightActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(LightActor);
}

bool UCreatePointLightAction::Execute_Implementation(bool bEmitRecord)
{
	APointLight* LightActor;
	UPoly_CreationFunctions::CreatePointLight(this, LightActor);

	// attach created actor to ActiveScene (ToDo: wrap into function library)
	USceneSubsystemFunctions::AddToActiveScene(this, LightActor);

	if (IsValid(LightActor))
		this->Submit();
	else
		this->Discard();
	return IsValid(LightActor);
}
