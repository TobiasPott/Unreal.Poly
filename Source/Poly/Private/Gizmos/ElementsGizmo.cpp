// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/ElementsGizmo.h"
#include "Functions/Poly_UIFunctions.h"
#include "UI/PolyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"

// Sets default values
AElementsGizmo::AElementsGizmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AElementsGizmo::BeginPlay()
{
	Super::BeginPlay();

	// create new request if none is available yet
	Request = NewObject<USelectionRequestBase>(this);


}

void AElementsGizmo::Setup(EActorSelectionRequestMode InMarqueeMode, bool bInIncludeOnlyEnclosed, bool bInDisableOnFinish)
{
	MarqueeMode = InMarqueeMode;
	bIncludeOnlyEnclosed = bInIncludeOnlyEnclosed;
	bDisableOnFinish = bInDisableOnFinish;
}

void AElementsGizmo::SetEnabled(const bool bInEnable)
{
	if (!bIsEnabled && bInEnable)
	{
		// bind primary input key pressed/released events
		this->EnableInput(UGameplayStatics::GetPlayerController(this, PlayerIndex));

		FInputActionBinding& IKA_Pressed = InputComponent->BindAction(InputAction, EInputEvent::IE_Pressed, this, &AElementsGizmo::OnInputKey_Pressed);
		IKA_Pressed.bConsumeInput = true;
		IKA_Pressed.bExecuteWhenPaused = true;
		FInputActionBinding& IKA_Released = InputComponent->BindAction(InputAction, EInputEvent::IE_Released, this, &AElementsGizmo::OnInputKey_Released);
		IKA_Released.bConsumeInput = true;
		IKA_Released.bExecuteWhenPaused = true;

		// bind mouse axes events (to conume their input from other receivers)
		FInputVectorAxisBinding& Mouse2D_Axis = InputComponent->BindVectorAxis("Mouse2D");
		Mouse2D_Axis.AxisDelegate.BindDelegate(this, &AElementsGizmo::OnMouse2D);
		Mouse2D_Axis.bConsumeInput = false;
		Mouse2D_Axis.bExecuteWhenPaused = true;

		FInputAxisKeyBinding& MouseX_Axis = InputComponent->BindAxisKey("MouseX");
		MouseX_Axis.AxisDelegate.BindDelegate(this, &AElementsGizmo::OnMouseX);
		MouseX_Axis.bConsumeInput = false;
		MouseX_Axis.bExecuteWhenPaused = true;
		FInputAxisKeyBinding& MouseY_Axis = InputComponent->BindAxisKey("MouseY");
		MouseY_Axis.AxisDelegate.BindDelegate(this, &AElementsGizmo::OnMouseY);
		MouseY_Axis.bConsumeInput = false;
		MouseY_Axis.bExecuteWhenPaused = true;

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

void AElementsGizmo::SetGizmoHidden(const bool bHiddenInGame)
{
	Super::SetGizmoHidden(bHiddenInGame);
	this->SetEnabled(!bHiddenInGame);
}

void AElementsGizmo::SetTargets(const TArray<AActor*>& Targets)
{
	this->Selection.Reset();
	for (int i = 0; i < Targets.Num(); i++)
	{
		AActor* Target = Targets[i];
		UBaseDynamicMeshComponent* BaseDMC = Target->GetComponentByClass<UBaseDynamicMeshComponent>();
		if (IsValid(BaseDMC))
			this->Selection.Add(Target, FGeometryScriptMeshSelection());
	}
}


void AElementsGizmo::OnInputKey_Pressed(FKey InKey)
{
	UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, FirstPoint);
	SecondPoint = FirstPoint;

	Request = NewObject<USelectionRequestBase>(this);
	Request->Mode = MarqueeMode;
	Request->FirstPoint = FirstPoint;
	Request->SecondPoint = FirstPoint;

	APolyHUD* Hud = Cast<APolyHUD>(UGameplayStatics::GetPlayerController(this, PlayerIndex)->GetHUD());
	if (IsValid(Hud))
	{
		Hud->QueueBaseRequest(Request);
	}
	bIsMousePressed = true;
}

void AElementsGizmo::OnInputKey_Released(FKey InKey)
{
	if (bIsMousePressed && IsValid(Request))
	{
		bIsMousePressed = false;
		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->SecondPoint = SecondPoint;


		Request->Submit();
		this->OnFinished();
	}
}

void AElementsGizmo::OnMouseX(float AxisValue)
{
}

void AElementsGizmo::OnMouseY(float AxisValue)
{
}

void AElementsGizmo::OnMouse2D(FVector AxisValue)
{
	if (bIsMousePressed)
	{
		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->SecondPoint = SecondPoint;
	}
}

void AElementsGizmo::OnFinished()
{
	if (this->Finished.IsBound())
		this->Finished.Broadcast(this);

	if (bDisableOnFinish)
		this->SetEnabled(false);
	this->Request = nullptr;
}

void AElementsGizmo::Clear()
{
	this->Selection.Reset();
	this->Request = nullptr;
}
