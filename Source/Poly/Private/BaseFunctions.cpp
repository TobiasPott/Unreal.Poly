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

bool UBaseFunctions::GetMousePosition(const UObject* WorldContext, const int32 PlayerIndex, FVector2D& OutPosition)
{
	double X;
	double Y;
	bool bSuccess = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex)->GetMousePosition(X, Y);
	OutPosition = FVector2D(X, Y);
	return bSuccess;
}

void UBaseFunctions::GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize)
{
	OutOrigin = FVector2D(FMath::Min(FirstPoint.X, SecondPoint.X), FMath::Min(FirstPoint.Y, SecondPoint.Y));
	OutSize = (SecondPoint - FirstPoint).GetAbs();
}

void UBaseFunctions::SelectWithSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD)
{
	TArray<AActor*> OutActors;
	HUD->GetActorsInSelectionRectangle(Request->FilterClass, Request->FirstPoint, Request->SecondPoint, OutActors,
		Request->bIncludeNonCollider, Request->bOnlyEnclosed);
	Request->SetActors(OutActors);
	Request->OnFinished();
}

void UBaseFunctions::DrawRequest(UActorSelectionRequest* Request, AHUD* HUD)
{
	FVector2D Origin;
	FVector2D Size;
	switch (Request->Mode)
	{
	case EActorSelectionRequestMode::Marquee:
		GetRectOriginAndSize(Request->FirstPoint, Request->SecondPoint, Origin, Size);
		HUD->DrawRect(FLinearColor(1, 1, 1, 0.25), Origin.X, Origin.Y, Size.X, Size.Y);
		break;

	case EActorSelectionRequestMode::Click:
	case EActorSelectionRequestMode::Trace:
		break;
	}
}
