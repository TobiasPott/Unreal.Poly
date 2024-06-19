// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/EditActorActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "EnumTypes.h"


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
		if (this->Space == ETransformSpace::TS_Local)
		{
			for (int i = 0; i < ActiveSelection.Num(); i++)
			{
				AActor* Selected = ActiveSelection[i]->GetSelectedActor();
				Selected->AddActorLocalOffset(this->DeltaTransform.GetLocation());
				Selected->AddActorLocalRotation(this->DeltaTransform.GetRotation());
				Selected->SetActorRelativeScale3D(Selected->GetActorRelativeScale3D() + this->DeltaTransform.GetScale3D());
			}
		}
		else
		{
			for (int i = 0; i < ActiveSelection.Num(); i++)
			{
				AActor* Selected = ActiveSelection[i]->GetSelectedActor();
				Selected->AddActorWorldOffset(this->DeltaTransform.GetLocation());
				Selected->AddActorWorldRotation(this->DeltaTransform.GetRotation());
				Selected->SetActorScale3D(Selected->GetActorScale3D() + this->DeltaTransform.GetScale3D());
			}
		}
		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}

inline void UTransformSelectionAction::SetLocation(FVector InLocation, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetLocation(InLocation);
}

inline void UTransformSelectionAction::SetRotation(FQuat InRotation, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetRotation(InRotation);
}

inline void UTransformSelectionAction::SetScale3D(FVector InScale, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetScale3D(InScale);
}
