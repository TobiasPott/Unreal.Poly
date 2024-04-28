// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/GizmoBaseActor.h"

// Sets default values
AGizmoBaseActor::AGizmoBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

}
