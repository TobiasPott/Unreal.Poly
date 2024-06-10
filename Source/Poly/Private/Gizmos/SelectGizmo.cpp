// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/SelectGizmo.h"
#include "Functions/Poly_UIFunctions.h"
#include "Functions/Poly_MeshSelectionFunctions.h"
#include "Selection/SelectorSubsystem.h"
#include "Selection/SelectorBase.h"
#include "UI/PolyHUD.h"
#include "EnumTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Actions/Categories/SelectionActions.h"
#include "Actions/ActionRunner.h"
#include "Functions/Poly_BaseFunctions.h"

// Sets default values
ASelectGizmo::ASelectGizmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASelectGizmo::BeginPlay()
{
	Super::BeginPlay();

	// create new request if none is available yet
	Request = NewObject<USelectionRequest>(this);


}

void ASelectGizmo::Setup(ESelectionRequestMode InMarqueeMode, UClass* InFilterClass, bool bInIncludeNonCollider, bool bInIncludeOnlyEnclosed,
	bool bInDisableOnFinish)
{
	MarqueeMode = InMarqueeMode;
	FilterClass = InFilterClass;
	bIncludeNonCollider = bInIncludeNonCollider;
	bIncludeOnlyEnclosed = bInIncludeOnlyEnclosed;
	bDisableOnFinish = bInDisableOnFinish;
}

void ASelectGizmo::SetEnabled(const bool bInEnable)
{
	if (!bIsEnabled && bInEnable)
	{
		// bind primary input key pressed/released events
		this->EnableInput(UGameplayStatics::GetPlayerController(this, PlayerIndex));

		FInputActionBinding& IKA_Pressed = InputComponent->BindAction(InputAction, EInputEvent::IE_Pressed, this, &ASelectGizmo::OnInputKey_Pressed);
		IKA_Pressed.bConsumeInput = true;
		IKA_Pressed.bExecuteWhenPaused = true;
		FInputActionBinding& IKA_Released = InputComponent->BindAction(InputAction, EInputEvent::IE_Released, this, &ASelectGizmo::OnInputKey_Released);
		IKA_Released.bConsumeInput = true;
		IKA_Released.bExecuteWhenPaused = true;

		// bind mouse axes events (to conume their input from other receivers)
		FInputVectorAxisBinding& Mouse2D_Axis = InputComponent->BindVectorAxis("Mouse2D");
		Mouse2D_Axis.AxisDelegate.BindDelegate(this, &ASelectGizmo::OnMouse2D);
		Mouse2D_Axis.bConsumeInput = false;
		Mouse2D_Axis.bExecuteWhenPaused = true;

		FInputAxisKeyBinding& MouseX_Axis = InputComponent->BindAxisKey("MouseX");
		MouseX_Axis.AxisDelegate.BindDelegate(this, &ASelectGizmo::OnMouseX);
		MouseX_Axis.bConsumeInput = false;
		MouseX_Axis.bExecuteWhenPaused = true;
		FInputAxisKeyBinding& MouseY_Axis = InputComponent->BindAxisKey("MouseY");
		MouseY_Axis.AxisDelegate.BindDelegate(this, &ASelectGizmo::OnMouseY);
		MouseY_Axis.bConsumeInput = false;
		MouseY_Axis.bExecuteWhenPaused = true;

		//if (!IsValid(Request))
		//	Request = NewObject<UActorSelectionRequest>(this);
	}
	else if (bIsEnabled && !bInEnable)
	{

		InputComponent->RemoveAxisBinding("Mouse2D");
		InputComponent->RemoveAxisBinding("MouseX");
		InputComponent->RemoveAxisBinding("MouseY");

		InputComponent->RemoveActionBinding(InputAction, EInputEvent::IE_Pressed);
		InputComponent->RemoveActionBinding(InputAction, EInputEvent::IE_Released);

		// bind primary input key pressed/released events
		this->DisableInput(UGameplayStatics::GetPlayerController(this, PlayerIndex));
	}

	bIsEnabled = bInEnable;
}

void ASelectGizmo::SetGizmoHidden(bool bHiddenInGame)
{
	Super::SetGizmoHidden(bHiddenInGame);
	this->SetEnabled(!bHiddenInGame);
}

void ASelectGizmo::SetSelectionMode(EPolySelectionMode InSelectionMode)
{
	this->SelectionMode = InSelectionMode;
}

void ASelectGizmo::UpdateSelection()
{
	switch (this->SelectionMode)
	{
	case EPolySelectionMode::Deselect:
	{
		for (AActor* Actor : this->Request->Actors)
		{
			this->Selection.Remove(Actor);
			UPolySelection::RemoveByT(this->PolySelection, Actor);
		}
		break;
	}
	case EPolySelectionMode::Select:
	{
		for (AActor* Actor : this->Request->Actors)
		{
			this->Selection.AddUnique(Actor);
			UPolySelection::AddByActorT(this->PolySelection, Actor);
		}
		break;
	}
	case EPolySelectionMode::Replace:
	default:
	{
		this->Selection.Reset(this->Request->Count());
		this->Selection.Append(this->Request->Actors);

		this->PolySelection.Reset(0);
		UPolySelection::AddByActorsT(this->PolySelection, this->Selection);
		break;
	}
	}

	//UE_LOG(LogPolyTemp, Warning, TEXT("UpdateSelection()..."))
	//	USetSelectionAction* SetSelectionAction = NewObject<USetSelectionAction>(this);
	//SetSelectionAction->SelectorName = USelectorNames::Actors;
	//UPolySelection::AddIdsTo(this->PolySelection, SetSelectionAction->Ids, true);
	//// ToDo: @tpott: Consolidate this to allow C++ to run action with static functionn which uses GetOrCreate to get a runner
	////				May consider adding a static instance and cache it (though this would break use of existing GetOrCreate of action runners
	//AActor* ActionRunnerActor;
	//UPoly_BaseFunctions::GetOrCreateActor(this, AActionRunner::StaticClass(), ActionRunnerActor);
	//AActionRunner* ActionRunner = Cast<AActionRunner>(ActionRunnerActor);
	//ActionRunner->Run(SetSelectionAction);
	//// ToDo: @tpott: insert use of setselectionaction here
	//// Add poly selection to selector 'Actors'
	//ASelectorBase* Selector;
	//if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>()->GetSelector(this, USelectorNames::Actors, Selector))
	//{
	//	Selector->ReplaceAll(this->PolySelection);
	//}

}


void ASelectGizmo::OnInputKey_Pressed(FKey InKey)
{
	UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, FirstPoint);
	SecondPoint = FirstPoint;

	Request = NewObject<USelectionRequest>(this);
	Request->Init(MarqueeMode, FirstPoint, FirstPoint, FilterClass, bIncludeNonCollider, bIncludeOnlyEnclosed);

	APolyHUD* Hud = Cast<APolyHUD>(UGameplayStatics::GetPlayerController(this, PlayerIndex)->GetHUD());
	if (IsValid(Hud))
	{
		Hud->QueueRequest(Request);
	}
	bIsMousePressed = true;
}

void ASelectGizmo::OnInputKey_Released(FKey InKey)
{
	if (bIsMousePressed && IsValid(Request))
	{
		bIsMousePressed = false;
		const APlayerController* PC = UGameplayStatics::GetPlayerController(this, this->PlayerIndex);
		const bool bShift = PC->IsInputKeyDown(EKeys::LeftShift);
		const bool bCtrl = PC->IsInputKeyDown(EKeys::LeftControl);
		if (bCtrl) this->SetSelectionMode(EPolySelectionMode::Deselect);
		else if (bShift) this->SetSelectionMode(EPolySelectionMode::Select);
		else this->SetSelectionMode(EPolySelectionMode::Replace);

		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->UpdateSecondPoint(SecondPoint);

		Request->Submit();
		Request->Finished.AddDynamic(this, &ASelectGizmo::OnRequestFinished);
	}
}

void ASelectGizmo::OnMouseX(float AxisValue)
{
}

void ASelectGizmo::OnMouseY(float AxisValue)
{
}

void ASelectGizmo::OnMouse2D(FVector AxisValue)
{
	if (bIsMousePressed)
	{
		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->UpdateSecondPoint(SecondPoint);
	}
}

void ASelectGizmo::OnRequestFinished(USelectionRequest* InRequest, bool bSuccess)
{
	Request->Finished.RemoveDynamic(this, &ASelectGizmo::OnRequestFinished);
	if (bSuccess)
	{
		this->UpdateSelection();
	}

	// ToDo: @tpott: check for empty selection in case nothing was selected and skip creating new action
	UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished()..."));
	USetSelectionAction* SetSelectionAction = NewObject<USetSelectionAction>(this);
	SetSelectionAction->SelectorName = USelectorNames::Actors;
	//UPolySelection::AddIdsTo(this->PolySelection, SetSelectionAction->Ids, true);
	for (int i = 0; i < this->PolySelection.Num(); i++)
	{
		SetSelectionAction->Ids.Add(this->PolySelection[i]->TargetId);
	}

	UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished.1: %d/%d"), this->PolySelection.Num(), SetSelectionAction->Ids.Num());
	// ToDo: @tpott: Consolidate this to allow C++ to run action with static functionn which uses GetOrCreate to get a runner
	//				May consider adding a static instance and cache it (though this would break use of existing GetOrCreate of action runners
	AActor* ActionRunnerActor;
	UPoly_BaseFunctions::GetOrCreateActor(this, AActionRunner::StaticClass(), ActionRunnerActor);
	AActionRunner* ActionRunner = Cast<AActionRunner>(ActionRunnerActor);
	ActionRunner->Run(SetSelectionAction);


	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, SetSelectionAction->SelectorName, Selector))
	{
		UE_LOG(LogPolyTemp, Warning, TEXT("OnRequestFinished.End: %d/%d"), this->PolySelection.Num(), SetSelectionAction->Ids.Num());
	}
	//// ToDo: @tpott: insert use of setselectionaction here
	//// Add poly selection to selector 'Actors'
	//ASelectorBase* Selector;
	//if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>()->GetSelector(this, USelectorNames::Actors, Selector))
	//{
	//	Selector->ReplaceAll(this->PolySelection);
	//}
	

	this->OnFinished();
}

void ASelectGizmo::OnFinished()
{
	if (Finished.IsBound())
		Finished.Broadcast(this->Request, this->Request->IsNotEmpty());

	if (bDisableOnFinish)
		this->SetEnabled(false);
	this->Request = nullptr;
}

void ASelectGizmo::Clear()
{
	this->Selection.Reset(this->Request->Count());
	this->Request = nullptr;
}
