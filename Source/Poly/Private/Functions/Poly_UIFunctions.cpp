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

bool UPoly_UIFunctions::GetScreenRay(const UObject* WorldContext, const int32 PlayerIndex, const FVector2D& ScreenPosition, FVector& WorldPosition, FVector& WorldDirection)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex);
	return PC->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);
}

bool UPoly_UIFunctions::GetScreenRaySegment(const UObject* WorldContext, const int32 PlayerIndex, const FVector2D& ScreenPosition, FVector& WorldStart, FVector& WorldEnd, const float Distance)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex);
	bool bSuccess = PC->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldStart, WorldEnd);
	WorldEnd = WorldStart + (WorldEnd * Distance);
	return bSuccess;
}

void UPoly_UIFunctions::GetScreenRectWorldCorners(const UObject* WorldContext, const int32 PlayerIndex, const FVector2D& BottomLeft, const FVector2D& BottomRight, const FVector2D& TopRight, const FVector2D& TopLeft, TArray<FVector>& OutNearCorners, TArray<FVector>& OutFarCorners, const float Distance)
{
	OutNearCorners.Reset(4);
	OutFarCorners.Reset(4);
	FVector BLS, BLE, BRS, BRE, TRS, TRE, TLS, TLE;

	UPoly_UIFunctions::GetScreenRaySegment(WorldContext, PlayerIndex, BottomLeft, BLS, BLE, Distance);
	UPoly_UIFunctions::GetScreenRaySegment(WorldContext, PlayerIndex, BottomRight, BRS, BRE, Distance);
	UPoly_UIFunctions::GetScreenRaySegment(WorldContext, PlayerIndex, TopRight, TRS, TRE, Distance);
	UPoly_UIFunctions::GetScreenRaySegment(WorldContext, PlayerIndex, TopLeft, TLS, TLE, Distance);

	OutNearCorners.Add(BLS);
	OutNearCorners.Add(BRS);
	OutNearCorners.Add(TRS);
	OutNearCorners.Add(TLS);

	OutFarCorners.Add(BLE);
	OutFarCorners.Add(BRE);
	OutFarCorners.Add(TRE);
	OutFarCorners.Add(TLE);
}


void UPoly_UIFunctions::GetRectOriginAndSize(const FVector2D FirstPoint, const FVector2D SecondPoint, FVector2D& OutOrigin, FVector2D& OutSize)
{
	OutOrigin = FVector2D(FMath::Min(FirstPoint.X, SecondPoint.X), FMath::Min(FirstPoint.Y, SecondPoint.Y));
	OutSize = (SecondPoint - FirstPoint).GetAbs();
}

void UPoly_UIFunctions::GetRectCorners(const FVector2D Origin, const FVector2D Size, FVector2D& OutBottomLeft, FVector2D& OutBottomRight, FVector2D& OutTopRight, FVector2D& OutTopLeft)
{
	OutBottomLeft = Origin + FVector2D(0, Size.Y);
	OutBottomRight = Origin + Size;
	OutTopLeft = Origin;
	OutTopRight = Origin + FVector2D(Size.X, 0);
}

void UPoly_UIFunctions::SelectWithSelectionRequest(UActorSelectionRequest* Request, AHUD* HUD)
{
	switch (Request->Mode)
	{
	case ESelectionRequestMode::Trace:
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
	case ESelectionRequestMode::Click:
	case ESelectionRequestMode::Marquee:
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

void UPoly_UIFunctions::DrawRequest(USelectionRequestBase* Request, AHUD* HUD, const FLinearColor& RectColor)
{
	FVector2D Origin;
	FVector2D Size;
	switch (Request->Mode)
	{
	case ESelectionRequestMode::Marquee:
		GetRectOriginAndSize(Request->FirstPoint, Request->SecondPoint, Origin, Size);
		HUD->DrawRect(RectColor, Origin.X, Origin.Y, Size.X, Size.Y);
		break;

	case ESelectionRequestMode::Click:
	case ESelectionRequestMode::Trace:
		break;
	}
}
