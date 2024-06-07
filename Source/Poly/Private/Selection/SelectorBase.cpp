// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectorBase.h"
#include "Selection/SelectorVisualiserBase.h"
#include "Functions/Poly_SelectorFunctions.h"
#include "Functions/Poly_ActorFunctions.h"
#include "Engine/World.h"

// Sets default values
ASelectorBase::ASelectorBase()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	DefaultSceneRoot = UPoly_ActorFunctions::CreateDefaultSceneComponent<USceneComponent>(this, "DefaultSceneRoot", EComponentMobility::Static);
}

// Called when the game starts or when spawned
void ASelectorBase::BeginPlay()
{
	Super::BeginPlay();

	// set/init visualiser type
	this->SetVisualiser(this->VisualiserClass);
}


void ASelectorBase::ClearSelection()
{
	bool bIsSelected = false;
	for (int i = Selection.Num() - 1; i >= 0; i--)
	{
		UPolySelection* Selected = Selection[i];
		if (IsValid(Selected))
			this->Deselect(Selected, bIsSelected);
	}
	Selection.Empty(0);
}

void ASelectorBase::SetVisualiser(TSubclassOf<ASelectorVisualiserBase> NewVisualiserClass)
{
	if (IsValid(NewVisualiserClass))
	{
		this->VisualiserClass = NewVisualiserClass;
		FTransform Transform = FTransform();
		UClass* Class = this->VisualiserClass.Get();
		UWorld* World = this->GetWorld();

		FActorSpawnParameters Params = FActorSpawnParameters();
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// spawn and init visualiser
		ASelectorVisualiserBase* Visualiser = World->SpawnActor<ASelectorVisualiserBase>(Class, Params);
		Visualiser->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
		Visualiser->Selector = this;
		Visualiser->Init();
	}
}

