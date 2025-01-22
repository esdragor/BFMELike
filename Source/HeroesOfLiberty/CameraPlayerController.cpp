// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ACameraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MySubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!MySubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No Enhanced Input Subsystem found"));
		return;
	}
	MySubsystem->ClearAllMappings();
	MySubsystem->AddMappingContext(MyInputMapping, 0);
}
