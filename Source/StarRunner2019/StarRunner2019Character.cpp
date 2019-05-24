// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019Character.h"
#include "Engine/EngineBaseTypes.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AStarRunner2019Character

AStarRunner2019Character::AStarRunner2019Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	isTurnable = false;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 0.0f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void AStarRunner2019Character::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStarRunner2019Character::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AStarRunner2019Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStarRunner2019Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStarRunner2019Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStarRunner2019Character::LookUpAtRate);

	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AStarRunner2019Character::TurnLeft);
	PlayerInputComponent->BindAction("TurnLeft", IE_Released , this, &AStarRunner2019Character::TurnLeft);

	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AStarRunner2019Character::TurnRight);
	PlayerInputComponent->BindAction("TurnRight", IE_Released, this, &AStarRunner2019Character::TurnRight);
}

void AStarRunner2019Character::TurnLeft() {
	if (isTurnable) {
		float turnAngle = -35.0f;
		AddControllerYawInput(turnAngle);
	}
}

void AStarRunner2019Character::TurnRight() {
	if (isTurnable) {
		float turnAngle = 35.0f;
		AddControllerYawInput(turnAngle);
	}
}

void AStarRunner2019Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AStarRunner2019Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AStarRunner2019Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStarRunner2019Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
