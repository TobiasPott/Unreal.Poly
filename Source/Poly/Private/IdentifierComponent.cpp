// Fill out your copyright notice in the Description page of Project Settings.


#include "IdentifierComponent.h"

int32 UIdentifierComponent::NextId = 0;

// Sets default values for this component's properties
UIdentifierComponent::UIdentifierComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UIdentifierComponent::SetAutoIdentifier(const FString& NewDisplayName)
{
	this->SetIdentifier(UIdentifierComponent::GetNextId(), NewDisplayName);
}

void UIdentifierComponent::SetIdentifier(const int32& NewId, const FString& NewDisplayName)
{
	this->Id = NewId;
	this->DisplayName = NewDisplayName;
}

int32 UIdentifierComponent::GetNextId()
{
	return UIdentifierComponent::NextId++;
}

