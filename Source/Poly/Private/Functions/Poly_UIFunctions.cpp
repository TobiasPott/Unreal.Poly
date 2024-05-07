// Fill out your copyright notice in the Description page of Project Settings.


#include "Functions/Poly_UIFunctions.h"
#include "Kismet/GameplayStatics.h"


bool UPoly_UIFunctions::GetMousePosition(const UObject* WorldContext, const int32 PlayerIndex, FVector2D& OutPosition)
{
	double X;
	double Y;
	bool bSuccess = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex)->GetMousePosition(X, Y);
	OutPosition = FVector2D(X, Y);
	return bSuccess;
}

bool UPoly_UIFunctions::GetMouseRay(const UObject* WorldContext, const int32 PlayerIndex, FVector& WorldPosition, FVector& WorldDirection)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex);
	return PC->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
}

bool UPoly_UIFunctions::GetMouseRaySegment(const UObject* WorldContext, const int32 PlayerIndex, FVector& WorldStart, FVector& WorldEnd, const float Distance)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex);
	bool bSuccess = PC->DeprojectMousePositionToWorld(WorldStart, WorldEnd);
	WorldEnd = WorldStart + (WorldEnd * Distance);
	return bSuccess;
}


void UPoly_UIFunctions::GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize)
{
	OutOrigin = FVector2D(FMath::Min(FirstPoint.X, SecondPoint.X), FMath::Min(FirstPoint.Y, SecondPoint.Y));
	OutSize = (SecondPoint - FirstPoint).GetAbs();
}

void UPoly_UIFunctions::SelectWithSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD)
{
	switch (Request->Mode)
	{
	case EActorSelectionRequestMode::Trace:
	{
		FVector Location;
		FVector Dir;
		if (UGameplayStatics::GetPlayerController(HUD, 0)->DeprojectScreenPositionToWorld(Request->SecondPoint.X, Request->SecondPoint.Y, Location, Dir))
		{
			ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(Request->TraceChannel);

			static const FName LineTraceSingleName(TEXT("LineTraceSingle"));
			FCollisionQueryParams Params(LineTraceSingleName, SCENE_QUERY_STAT_ONLY(KismetTraceUtils), Request->bTraceComplex);
			Params.bReturnPhysicalMaterial = false;
			Params.bReturnFaceIndex = false;

			UWorld* World = GEngine->GetWorldFromContextObject(HUD, EGetWorldErrorMode::LogAndReturnNull);
			FHitResult OutHit;
			const bool bHit = World ? World->LineTraceSingleByChannel(OutHit, Location, Location + (Dir * Request->TraceDistance), CollisionChannel, Params) : false;

			if (bHit && OutHit.GetActor()->IsA(Request->FilterClass))
				Request->SetActor(OutHit.GetActor());
		}
	}
	break;
	case EActorSelectionRequestMode::Click:
	case EActorSelectionRequestMode::Marquee:
	{
		TArray<AActor*> OutActors;
		HUD->GetActorsInSelectionRectangle(Request->FilterClass, Request->FirstPoint, Request->SecondPoint, OutActors,
			Request->bIncludeNonCollider, Request->bOnlyEnclosed);
		Request->SetActors(OutActors);
		break;
	}

	}
	Request->OnFinished();
}

void UPoly_UIFunctions::DrawRequest(USelectionRequestBase* Request, AHUD* HUD)
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
