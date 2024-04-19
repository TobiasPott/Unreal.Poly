// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PolyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseFunctions.h"
#include "UIFunctions.h"

APolyHUD::APolyHUD()
{

}

void APolyHUD::QueueRequest(UActorSelectionRequest* Request)
{
	this->MarqueeRequests.AddUnique(Request);
}

void APolyHUD::ProcessRequests()
{
	for (int i = this->MarqueeRequests.Num() - 1; i >= 0; i--)
	{
		UActorSelectionRequest* Request = this->MarqueeRequests[i];
		UUIFunctions::DrawRequest(Request, this);

		if (Request->bSubmitted)
		{
			//switch (Request->Mode)
			//{
			//case EActorSelectionRequestMode::Trace:
			//{
			//	FVector Location;
			//	FVector Dir;
			//	if (UGameplayStatics::GetPlayerController(this, 0)->DeprojectScreenPositionToWorld(Request->SecondPoint.X, Request->SecondPoint.Y, Location, Dir))
			//	{
			//		ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(Request->TraceChannel);

			//		static const FName LineTraceSingleName(TEXT("LineTraceSingle"));


			//		FCollisionQueryParams Params(LineTraceSingleName, SCENE_QUERY_STAT_ONLY(KismetTraceUtils), Request->bTraceComplex);
			//		Params.bReturnPhysicalMaterial = false;
			//		Params.bReturnFaceIndex = false;

			//		UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
			//		FHitResult OutHit;
			//		const  bool bHit = World ? World->LineTraceSingleByChannel(OutHit, Location, Location + (Dir * Request->TraceDistance), CollisionChannel, Params) : false;

			//		if (bHit)
			//		{
			//			Request->SetActor(OutHit.GetActor());
			//		}
			//		Request->OnFinished();
			//	}
			//}
			//break;
			//case EActorSelectionRequestMode::Click:
			//case EActorSelectionRequestMode::Marquee:
			//{
			//	UUIFunctions::SelectWithRectSelectionRequest(Request, this);
			//	break;
			//}

			//}

			UUIFunctions::SelectWithRectSelectionRequest(Request, this);

			this->MarqueeRequests.Remove(Request);
		}
	}
}

