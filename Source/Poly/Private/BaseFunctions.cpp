// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFunctions.h"
#include "Kismet/GameplayStatics.h"

void UBaseFunctions::GetOrCreateActor(const UObject* WorldContext, TSubclassOf<AActor> InClass, AActor*& OutActor)
{
	// https://benui.ca/unreal/ufunction/

	AActor* Instance = UGameplayStatics::GetActorOfClass(WorldContext, InClass);
	if (!IsValid(Instance))
	{
		UClass* Class = InClass.Get();
		UWorld* World = WorldContext->GetWorld();
		OutActor = World->SpawnActor(Class);
	}
	else
	{
		OutActor = Instance;
	}
		
	
}
