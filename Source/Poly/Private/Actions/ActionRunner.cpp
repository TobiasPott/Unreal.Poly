// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionRunner.h"
#include <Functions/Poly_BaseFunctions.h>

// Sets default values
AActionRunner::AActionRunner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AActionRunner::OnActionSubmitted(UActionBase* Action)
{
	if (IsValid(Action))
	{
		this->Actions.Add(Action);
		if (PendingAction == Action)
		{
			PendingAction->Submitted.RemoveDynamic(this, &AActionRunner::OnActionSubmitted);
			PendingAction->Discarded.RemoveDynamic(this, &AActionRunner::OnActionDiscarded);
			PendingAction = nullptr;

			UE_LOG(LogTemp, Display, TEXT("Submitted action: '%s' total (%d)"), *Action->GetDescription(), Actions.Num());

			if (this->SubmittedAction.IsBound())
				this->SubmittedAction.Broadcast(Action);
		}
		else
		{
			FString Description = IsValid(PendingAction) ? PendingAction->GetDescription() : "<No Pending Action>";
			UE_LOG(LogTemp, Warning, TEXT("Submitted action was not finished. Pending action does not match or is not set: '%s' (total actions: %d)"), *PendingAction->GetDescription(), Actions.Num());
		}
	}
}

void AActionRunner::OnActionDiscarded(UActionBase* Action)
{
	if (this->DiscardedAction.IsBound())
		this->DiscardedAction.Broadcast(Action);
}

bool AActionRunner::Run(UActionBase* Action)
{
	if (IsValid(PendingAction))
	{
		PendingAction->Discard();
		PendingAction->Submitted.RemoveDynamic(this, &AActionRunner::OnActionSubmitted);
		PendingAction->Discarded.RemoveDynamic(this, &AActionRunner::OnActionDiscarded);
		PendingAction = nullptr;
	}
	// set new pending
	if (IsValid(Action))
	{
		PendingAction = Action;
		PendingAction->Submitted.AddDynamic(this, &AActionRunner::OnActionSubmitted);
		PendingAction->Discarded.AddDynamic(this, &AActionRunner::OnActionDiscarded);
		PendingAction->Execute();
		return true;
	}
	return false;
}

bool AActionRunner::RunOnAny(const UObject* WorldContext, UActionBase* InAction)
{
	// ToDo: @tpott: Add static member to track one single ActionRunner (or make one per player and have sort of a subsystem?)
	AActor* ActionRunnerActor;
	UPoly_BaseFunctions::GetOrCreateActor(WorldContext, AActionRunner::StaticClass(), ActionRunnerActor);
	AActionRunner* ActionRunner = Cast<AActionRunner>(ActionRunnerActor);
	if (IsValid(ActionRunner) && IsValid(InAction))
		return ActionRunner->Run(InAction);
	return false;
}

