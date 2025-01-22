// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCameraPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "DataAssets/GameCameraInputDataConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameCameraPawn::AGameCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 1000.0f; // Default zoom
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AGameCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = true;
	}
	// take the screen size
	UGameViewportClient* GameViewportClient = GetWorld()->GetGameViewport();
	if (GameViewportClient)
	{
		FVector2D ViewportSize;
		GameViewportClient->GetViewportSize(ViewportSize);
		ScreenSize.X = ViewportSize.X;
		ScreenSize.Y = ViewportSize.Y;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameViewportClient is null")));
	}
}

void AGameCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Movement = FVector(MovementInput, 0.0f) * MovementSpeed * DeltaTime;
	AddActorWorldOffset(Movement, true);
	HandleScreenEdgeMovement(DeltaTime);
}

void AGameCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(InputDataConfig->Zoom, ETriggerEvent::Triggered, this, &AGameCameraPawn::Zoom);
		Input->BindAction(InputDataConfig->Move, ETriggerEvent::Triggered, this, &AGameCameraPawn::Move);
	}
}

void AGameCameraPawn::Move(const FInputActionValue& Value)
{
	FVector2D Axis2DValue = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Axis2DValue: %s"), *Axis2DValue.ToString()));
	MovementInput.X = Axis2DValue.X;
	MovementInput.Y = Axis2DValue.Y;
}

void AGameCameraPawn::HandleScreenEdgeMovement(float DeltaTime)
{
	float MousePosX;
	float MousePosY;
	float CamDirectionX = 0;
	float CamDirectionY = 0;

	GEngine->GameViewport->GetViewportSize(ScreenSize);

	if (!GetController() || ScreenSize.X == 0 || ScreenSize.Y == 0)
	{
		AddActorWorldOffset(FVector(0, 0, 0));
		if (!GetController())
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GetController is null")));
		if (ScreenSize.X == 0)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ScreenSizeX is null")));
		if (ScreenSize.Y == 0)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ScreenSizeY is null")));
		return;
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosX, MousePosY);

	if (MousePosX <= margin)
	{
		CamDirectionY = 1;
	}
	if (MousePosY <= margin)
	{
		CamDirectionX = -1;
	}

	if (MousePosX >= ScreenSize.X - margin)
	{
		CamDirectionY = -1;
	}
	if (MousePosY >= ScreenSize.Y - margin)
	{
		CamDirectionX = 1;
	}

	AddActorWorldOffset(FVector(CamDirectionX, CamDirectionY, 0));
}

void AGameCameraPawn::Zoom(const FInputActionValue& Value)
{
	float Axis = Value.Get<float>();
	ZoomLevel = FMath::Clamp( (Axis > 0) ? (ZoomLevel - ZoomSpeed) : (ZoomLevel + ZoomSpeed), MinZoom, MaxZoom);
	SpringArm->TargetArmLength = ZoomLevel;
}

