// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SceneSubsystemFunctions.h"
#include "Scene/SceneSubsystem.h"
#include "Kismet/GameplayStatics.h"

void USceneSubsystemFunctions::AddToScene(const UObject* WorldContext, AActor* InActor, const FName InSceneName)
{
	USceneSubsystem* SceneSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<USceneSubsystem>();
	bool bSuccess = false;
	ASceneActor* Scene = SceneSubsystem->GetScene(WorldContext, InSceneName, bSuccess);
	if (bSuccess)
	{
		Scene->Add(InActor, true);
	}
}

void USceneSubsystemFunctions::AddToActiveScene(const UObject* WorldContext, AActor* InActor)
{
	USceneSubsystem* SceneSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<USceneSubsystem>();
	ASceneActor* Scene = SceneSubsystem->GetActiveScene();
	Scene->Add(InActor, true);
}

void USceneSubsystemFunctions::RemoveFromScene(const UObject* WorldContext, AActor* InActor, const FName InSceneName)
{
	USceneSubsystem* SceneSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<USceneSubsystem>();
	bool bSuccess = false;
	ASceneActor* Scene = SceneSubsystem->GetScene(WorldContext, InSceneName, bSuccess);
	if (bSuccess)
	{
		Scene->Remove(InActor, true);
	}
}

void USceneSubsystemFunctions::RemoveFromActiveScene(const UObject* WorldContext, AActor* InActor)
{
	USceneSubsystem* SceneSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<USceneSubsystem>();
	ASceneActor* Scene = SceneSubsystem->GetActiveScene();
	Scene->Remove(InActor, true);
}
