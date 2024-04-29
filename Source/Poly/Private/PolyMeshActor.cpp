// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyMeshActor.h"

APolyMeshActor::APolyMeshActor()
{
	SetCollision(true);
}

void APolyMeshActor::SetCollision(bool bIsEnabled)
{
	UDynamicMeshComponent* DMC = this->GetDynamicMeshComponent();
	DMC->SetCollisionEnabled(bIsEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	DMC->SetGenerateOverlapEvents(bIsEnabled);
	DMC->CollisionType = ECollisionTraceFlag::CTF_UseSimpleAndComplex;
	DMC->bEnableComplexCollision = bIsEnabled;
}
