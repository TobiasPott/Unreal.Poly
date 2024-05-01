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
		bool bSuccess = false;
		this->ActiveScene = GetScene(WorldContext, USceneNames::Default, bSuccess);

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

ASceneActor* USceneSubsystem::GetScene(const UObject* WorldContext, FName Name, bool& bSuccess)
{
	if (Name.IsNone())
	{
		bSuccess = false;
		return nullptr;
	}
	if (this->Scenes.Contains(Name))
	{
		bSuccess = true;
		return this->Scenes[Name];
	}
	bSuccess = false;
	return nullptr;
}

ASceneActor* USceneSubsystem::GetActiveScene()
{
	return this->ActiveScene;
}
