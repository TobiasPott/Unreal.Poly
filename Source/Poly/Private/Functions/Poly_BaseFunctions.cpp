// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_BaseFunctions.h"
#include "Kismet/GameplayStatics.h"

void UPoly_BaseFunctions::GetOrCreateActor(const UObject* WorldContext, TSubclassOf<AActor> InClass, AActor*& OutActor)
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

void UPoly_BaseFunctions::SetActorHidden(AActor* Target, const bool bHiddenInGame)
{
	if (IsValid(Target))
	{
		Target->SetActorHiddenInGame(bHiddenInGame);
		Target->SetActorEnableCollision(!bHiddenInGame);
	}
}
