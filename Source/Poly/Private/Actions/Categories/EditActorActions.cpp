// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/EditActorActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "EnumTypes.h"
#include "Functions/Poly_ActorFunctions.h"


bool UDestroyActorsAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<UPolySelection*> SelectedActors = Selector->Selection;
		UE_LOG(LogPolyTemp, Warning, TEXT("Destroy Selected: %d"), SelectedActors.Num());
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			UPolySelection* Selected = SelectedActors[i];
			bool IsSelected;
			Selector->Deselect(Selected, IsSelected);
			// destroy actor
			if (IsValid(Selected->GetSelectedActor()))
				Selected->GetSelectedActor()->Destroy();
		}
		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}


bool UTransformSelectionAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		const TArray<UPolySelection*> ActiveSelection = Selector->Selection;
		for (int i = 0; i < ActiveSelection.Num(); i++)
		{
			UPoly_ActorFunctions::AddActorTransform(ActiveSelection[i]->GetSelectedActor(), DeltaTransform, this->Space);
		}

		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}

void UTransformSelectionAction::SetLocation(FVector InLocation, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetLocation(InLocation);
}

void UTransformSelectionAction::SetRotation(FQuat InRotation, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetRotation(InRotation);
}

void UTransformSelectionAction::SetRotation(FRotator InRotation, bool bClearTransform)
{
	SetRotation(InRotation.Quaternion(), bClearTransform);
}

void UTransformSelectionAction::SetScale3D(FVector InScale, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetScale3D(InScale);
}
