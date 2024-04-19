// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PolyHUD.h"
#include "BaseFunctions.h"

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
		UBaseFunctions::DrawRequest(Request, this);

		if (Request->bSubmitted)
		{
			UBaseFunctions::SelectWithSelectionRequest(Request, this);
			this->MarqueeRequests.Remove(Request);
		}
	}
}

