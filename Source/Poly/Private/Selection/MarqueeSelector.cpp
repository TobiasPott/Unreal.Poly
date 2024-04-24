// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/MarqueeSelector.h"
#include "Functions/Poly_UIFunctions.h"
#include "UI/PolyHUD.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMarqueeSelector::AMarqueeSelector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMarqueeSelector::BeginPlay()
{
	Super::BeginPlay();

	// create new request if none is available yet
	Request = NewObject<UActorSelectionRequest>(this);


}

void AMarqueeSelector::Setup(EActorSelectionRequestMode InMarqueeMode, UClass* InFilterClass, bool bInIncludeNonCollider, bool bInIncludeOnlyEnclosed,
	bool bInDisableOnFinish)
{
	MarqueeMode = InMarqueeMode;
	FilterClass = InFilterClass;
	bIncludeNonCollider = bInIncludeNonCollider;
	bIncludeOnlyEnclosed = bInIncludeOnlyEnclosed;
	bDisableOnFinish = bInDisableOnFinish;
}

void AMarqueeSelector::SetEnabled(const bool bInEnable)
{
	if (!bIsEnabled && bInEnable)
	{
		// bind primary input key pressed/released events
		this->EnableInput(UGameplayStatics::GetPlayerController(this, PlayerIndex));

		FInputActionBinding& IKA_Pressed = InputComponent->BindAction(InputAction, EInputEvent::IE_Pressed, this, &AMarqueeSelector::OnInputKey_Pressed);
		IKA_Pressed.bConsumeInput = true;
		IKA_Pressed.bExecuteWhenPaused = true;
		FInputActionBinding& IKA_Released = InputComponent->BindAction(InputAction, EInputEvent::IE_Released, this, &AMarqueeSelector::OnInputKey_Released);
		IKA_Released.bConsumeInput = true;
		IKA_Released.bExecuteWhenPaused = true;

		// bind mouse axes events (to conume their input from other receivers)
		FInputVectorAxisBinding& Mouse2D_Axis = InputComponent->BindVectorAxis("Mouse2D");
		Mouse2D_Axis.AxisDelegate.BindDelegate(this, &AMarqueeSelector::OnMouse2D);
		Mouse2D_Axis.bConsumeInput = false;
		Mouse2D_Axis.bExecuteWhenPaused = true;

		FInputAxisKeyBinding& MouseX_Axis = InputComponent->BindAxisKey("MouseX");
		MouseX_Axis.AxisDelegate.BindDelegate(this, &AMarqueeSelector::OnMouseX);
		MouseX_Axis.bConsumeInput = false;
		MouseX_Axis.bExecuteWhenPaused = true;
		FInputAxisKeyBinding& MouseY_Axis = InputComponent->BindAxisKey("MouseY");
		MouseY_Axis.AxisDelegate.BindDelegate(this, &AMarqueeSelector::OnMouseY);
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


void AMarqueeSelector::OnInputKey_Pressed(FKey InKey)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnInputKey_Pressed"));
	UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, FirstPoint);
	SecondPoint = FirstPoint;

	Request = NewObject<UActorSelectionRequest>(this);
	Request->Init(MarqueeMode, FirstPoint, FirstPoint, FilterClass, bIncludeNonCollider, bIncludeOnlyEnclosed);

	APolyHUD* Hud = Cast<APolyHUD>(UGameplayStatics::GetPlayerController(this, PlayerIndex)->GetHUD());
	if (IsValid(Hud))
	{
		Hud->QueueRequest(Request);
	}
	bIsMousePressed = true;
}

void AMarqueeSelector::OnInputKey_Released(FKey InKey)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnInputKey_Released"));
	bIsMousePressed = false;
	UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
	Request->UpdateSecondPoint(SecondPoint);

	Request->Submit();
	Request->Finished.AddDynamic(this, &AMarqueeSelector::OnRequestFinished);
}

void AMarqueeSelector::OnMouseX(float AxisValue)
{
}

void AMarqueeSelector::OnMouseY(float AxisValue)
{
}

void AMarqueeSelector::OnMouse2D(FVector AxisValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Axis 2D %s"), *AxisValue.ToString());
	if (bIsMousePressed)
	{
		UPoly_UIFunctions::GetMousePosition(this, PlayerIndex, SecondPoint);
		Request->UpdateSecondPoint(SecondPoint);
	}
}

void AMarqueeSelector::OnRequestFinished(UActorSelectionRequest* InRequest, bool bSuccess)
{
	//Request = InRequest;
	Request->Finished.RemoveDynamic(this, &AMarqueeSelector::OnRequestFinished);
	//this->Request = InRequest;
	this->OnFinished();
}

void AMarqueeSelector::OnFinished()
{
	if (Finished.IsBound())
		Finished.Broadcast(this->Request, this->Request->IsNotEmpty());

	// ToDo: Consider making this behaviour configurable per instance to allow reuse
	if (bDisableOnFinish)
		this->SetEnabled(false);
	this->Request = nullptr;
}