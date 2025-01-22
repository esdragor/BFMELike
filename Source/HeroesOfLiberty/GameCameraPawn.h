// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameCameraPawn.generated.h"

struct FInputActionValue;
class UGameCameraInputDataConfig;

UCLASS()
class HEROESOFLIBERTY_API AGameCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AGameCameraPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void HandleScreenEdgeMovement(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	FVector2D MovementInput;
	float ZoomLevel;
	UPROPERTY(EditAnywhere, Category = "Camera|Settings")
	float MovementSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Settings")
	float ZoomSpeed = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Settings")
	float MinZoom = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Settings")
	float MaxZoom = 3000.0f;
	UPROPERTY()
	float margin = 15;
	UPROPERTY()
	FVector2D ScreenSize = FVector2D::ZeroVector;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UGameCameraInputDataConfig* InputDataConfig;
};
