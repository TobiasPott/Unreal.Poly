// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SceneSubsystem.h"

ASceneSubsystemRoot::ASceneSubsystemRoot()
{
	// disable tick
	PrimaryActorTick.bCanEverTick = false;

	// create new scene component and make it root component others attach to
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneComponent);
}



ASceneActor* USceneSubsystem::CreateScene(const UObject* WorldContext)
{
	AActor* NewActor = WorldContext->GetWorld()->SpawnActor(ASceneActor::StaticClass());
	return Cast<ASceneActor>(NewActor);
}

void USceneSubsystem::Init(const UObject* WorldContext)
{
	if (!bIsInitialised)
	{
		if (!IsValid(Root))
		{
			Root = WorldContext->GetWorld()->SpawnActor<ASceneSubsystemRoot>(ASceneSubsystemRoot::StaticClass());
		}

		TArray<FName> SelectorNames = { USceneNames::Default };
		for (int i = 0; i < SelectorNames.Num(); i++)
		{
			// Add selector (using index as stencil (starting with 1 (i+1))
			AddScene(SelectorNames[i]);
		}

		// set active scene
		GetScene(WorldContext, USceneNames::Default, this->ActiveScene);

		bIsInitialised = true;
	}
}

bool USceneSubsystem::HasScene(FName Name)
{
	return this->Scenes.Contains(Name);
}

ASceneActor* USceneSubsystem::AddScene(FName Name)
{
	if (!this->Scenes.Contains(Name))
	{
		ASceneActor* Scene = CreateScene(Root);
		Scene->AttachToActor(Root, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
		Scene->Name = Name;
		this->Scenes.Add(Name, Scene);
		return Scene;
	}
	return this->Scenes[Name];
}

bool USceneSubsystem::GetScene(const UObject* WorldContext, FName Name, ASceneActor*& OutScene)
{
	if (Name.IsNone())
	{
		OutScene = nullptr;
		return false;
	}
	if (this->Scenes.Contains(Name))
	{
		OutScene = this->Scenes[Name];
		return true;
	}
	return false;
}

ASceneActor* USceneSubsystem::GetActiveScene()
{
	return this->ActiveScene;
}
