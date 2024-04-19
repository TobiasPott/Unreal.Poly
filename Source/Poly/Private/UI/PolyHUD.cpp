// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PolyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Functions/Poly_UIFunctions.h"

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
		UPoly_UIFunctions::DrawRequest(Request, this);

		if (Request->bSubmitted)
		{
			UPoly_UIFunctions::SelectWithSelectionRequest(Request, this);
			this->MarqueeRequests.Remove(Request);
		}
	}
}

