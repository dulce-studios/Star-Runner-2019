// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019Character.h"
#include "HallwayActor.h"

#include "Components/BoxComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

#define BASE_SPEED 500.0f
#define MAX_SPEED 1500.0f

//////////////////////////////////////////////////////////////////////////
// AStarRunner2019Character

AStarRunner2019Character::AStarRunner2019Character()
{
	// Set size for collision capsule
	this->CharacterCapsuleComponent =  GetCapsuleComponent();
	this->CharacterCapsuleComponent->InitCapsuleSize(55.f, 96.0f);

	this->IsTurnable = false;
	this->WentLeft = false;
	this->HallwaysPassedCount = 0;

	this->MovementComponent = this->GetCharacterMovement();
	this->MovementComponent->MaxWalkSpeed = BASE_SPEED;

	// set our turn rates for input
	this->BaseTurnRate = 45.f;
	this->BaseLookUpRate = 45.f;

	// Create a CameraComponent
	this->FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	this->FirstPersonCameraComponent->SetupAttachment(this->CharacterCapsuleComponent);
	this->FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 0.0f); // Position the camera
	this->FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void AStarRunner2019Character::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	this->CharacterCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AStarRunner2019Character::OnOverlapBegin);
	this->CharacterCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AStarRunner2019Character::OnOverlapEnd);
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
	if (IsTurnable) {
		float turnAngle = -35.0f;
		this->AddControllerYawInput(turnAngle);
		this->WentLeft = true;
	}
}

void AStarRunner2019Character::TurnRight() {
	if (IsTurnable) {
		float turnAngle = 35.0f;
		this->AddControllerYawInput(turnAngle * BaseTurnRate);
		this->WentLeft = false;
	}
}

void AStarRunner2019Character::MoveForward(float Value) {
	this->AddMovementInput(GetActorForwardVector(), 1.0f);
}

void AStarRunner2019Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		this->AddMovementInput(GetActorRightVector(), Value);
	}
}

void AStarRunner2019Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	this->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStarRunner2019Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	this->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStarRunner2019Character::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void AStarRunner2019Character::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AHallwayActor::StaticClass())) {
		AHallwayActor* HallwayActor = Cast<AHallwayActor>(OtherActor);
		UHallwayJointComponent* HallwayJointComponent = Cast<UHallwayJointComponent>(HallwayActor->GetDefaultSubobjectByName(TEXT("HallwayJointComponent")));

		UBoxComponent* HallwayJointBoxComponent = Cast<UBoxComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("TriggerBox")));

		if (HallwayJointBoxComponent == OtherComp) {
			++this->HallwaysPassedCount;

			if (this->HallwaysPassedCount % 5 == 0 && this->MovementComponent->GetMaxSpeed() <= MAX_SPEED) this->MovementComponent->MaxWalkSpeed += 50;
		}
	}
}
