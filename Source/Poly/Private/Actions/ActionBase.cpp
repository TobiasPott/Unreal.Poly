// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/ActionBase.h"
#include "Actions/ActionRef.h"
#include "Actions/ActionCollection.h"
#include "Actions/Categories/EditMeshActions.h"
#include "Actions/Categories/CreateShapeActions.h"

UActionRef* UActionBase::GetUnique()
{
	UActionRef* Ref = NewObject<UActionRef>(this);
	Ref->Action = this;
	return Ref;
}

void UActionBase::CreateActionCollection(UObject* WorldContext, const FString InShortName, const FString InDescription, TArray<UActionBase*>& InActions, UActionCollection*& OutCollection)
{
	OutCollection = NewObject<UActionCollection>(WorldContext);
	OutCollection->SetShortNameAndDescription(InShortName, InDescription);
	OutCollection->AddItems(InActions);
}

void UActionBase::CreateEditMeshCollection(UObject* WorldContext, UActionCollection*& OutCollection)
{
	TArray<UActionBase*> Actions = {
		NewObject<UDeleteMeshElementsAction>(WorldContext),
		NewObject<UCreatePolygonsAction>(WorldContext),
		NewObject<UFlipNormalsAction>(WorldContext),
		NewObject<USubdivideMeshAction>(WorldContext),
		NewObject<UTessellateMeshAction>(WorldContext),
		NewObject<UInsetOutsetFacesAction>(WorldContext),
	};
	CreateActionCollection(WorldContext, "poly.EditMesh", "Edit Mesh action collection", Actions, OutCollection);
}

void UActionBase::CreateShapeCollection(UObject* WorldContext, UActionCollection*& OutCollection)
{
	TArray<UActionBase*> Actions = {
		NewObject<UCreateBoxAction>(WorldContext),
		NewObject<UCreateSphereAction>(WorldContext),
		NewObject<UCreateCylinderAction>(WorldContext),
		NewObject<UCreateConeAction>(WorldContext),
	};
	CreateActionCollection(WorldContext, "poly.Shapes", "Shapes action collection", Actions, OutCollection);
}

