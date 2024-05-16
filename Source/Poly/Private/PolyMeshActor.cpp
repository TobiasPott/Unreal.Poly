// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"

APolyMeshActor::APolyMeshActor()
{
	SetCollision(true);
	this->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	IdentifierComponent = CreateDefaultSubobject<UIdentifierComponent>(TEXT("IdentifierComponent"));
}

void APolyMeshActor::BeginPlay()
{
	Super::BeginPlay();
	IdentifierComponent->SetAutoIdentifier(UKismetSystemLibrary::GetObjectName(this));
}

void APolyMeshActor::SetCollision(bool bIsEnabled)
{
	UDynamicMeshComponent* DMC = this->GetDynamicMeshComponent();
	DMC->SetCollisionEnabled(bIsEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	DMC->SetGenerateOverlapEvents(bIsEnabled);
	DMC->CollisionType = ECollisionTraceFlag::CTF_UseSimpleAndComplex;
	DMC->bEnableComplexCollision = bIsEnabled;
}
