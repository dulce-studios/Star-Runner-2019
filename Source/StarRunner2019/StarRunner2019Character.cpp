// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019Character.h"
#include "HallwayActor.h"
#include "StarRunner2019HUD.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include <cmath>
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//TODO Use Enum to model left/right turn for this and hallways

constexpr float BASE_SPEED = 500;
constexpr float SPEED_STEP = 50;
constexpr float MAX_SPEED = 1500;

//////////////////////////////////////////////////////////////////////////
// AStarRunner2019Character

AStarRunner2019Character::AStarRunner2019Character()
{
	// Set size for collision capsule
	this->CharacterCapsuleComponent = GetCapsuleComponent();
	this->CharacterCapsuleComponent->InitCapsuleSize(55.f, 96.0f);

	this->bIsTurnable = false;
	this->HallwaysPassedCount = 0;

	this->bIsTurning = false;
	this->TurnDirection = EDirection::None;
	this->MovementComponent = this->GetCharacterMovement();
	this->MovementComponent->MaxWalkSpeed = BASE_SPEED;

	this->TimeElapsed = 0;

	// set our turn rates for input
	this->BaseTurnRate = 45.f;
	this->BaseLookUpRate = 45.f;

	// Create a CameraComponent
	this->FirstPersonCameraComponent = this->CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	this->FirstPersonCameraComponent->SetupAttachment(this->CharacterCapsuleComponent);
	this->FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 0.0f); // Position the camera
	this->FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void AStarRunner2019Character::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	this->CharacterCapsuleComponent->OnComponentEndOverlap.AddDynamic(
		this,
		&AStarRunner2019Character::OnOverlapEnd);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStarRunner2019Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
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
	PlayerInputComponent->BindAction("TurnLeft", IE_Released, this, &AStarRunner2019Character::TurnLeft);

	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AStarRunner2019Character::TurnRight);
	PlayerInputComponent->BindAction("TurnRight", IE_Released, this, &AStarRunner2019Character::TurnRight);
}

void AStarRunner2019Character::TurnLeft() {
	this->Turn(EDirection::Left);
}

void AStarRunner2019Character::TurnRight() {
	this->Turn(EDirection::Right);
}

void AStarRunner2019Character::Tick(float DeltaSeconds) {
	this->TimeElapsed += DeltaSeconds;
	UE_LOG(LogTemp, Warning, TEXT("HEEE %f"), this->TimeElapsed);

	AStarRunner2019HUD* StarRunnerHUD
		= Cast<AStarRunner2019HUD>(this->GetWorld()->GetFirstPlayerController()->GetHUD());

	StarRunnerHUD->SetElapsedTime(this->TimeElapsed);

	if (this->bIsTurning) {
		AController* PlayerController = this->GetController();
		const FRotator CurrentRotation = this->GetActorRotation();

		const float RInterpStopTolerance = 0.12;
		if (CurrentRotation.Equals(this->TargetRotation, RInterpStopTolerance)) {
			PlayerController->SetControlRotation(this->TargetRotation);
			this->bIsTurning = false;
		}
		else {
			const float InterpSpeed = 8;
			PlayerController->SetControlRotation(FMath::RInterpTo(
				CurrentRotation,
				this->TargetRotation,
				DeltaSeconds,
				InterpSpeed));
		}
	}
}

void AStarRunner2019Character::MoveForward(float Value) {
	const float InputOverride = 1;
	this->AddMovementInput(this->GetActorForwardVector(), InputOverride);
}

void AStarRunner2019Character::MoveRight(float Value)
{
	if (Value != 0)
	{
		// add movement in that direction
		this->AddMovementInput(this->GetActorRightVector(), Value);
	}
}

void AStarRunner2019Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	const float SecondsPerFrame = this->GetWorld()->GetDeltaSeconds();
	this->AddControllerYawInput(Rate * this->BaseTurnRate * SecondsPerFrame);
}

void AStarRunner2019Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	const float SecondsPerFrame = this->GetWorld()->GetDeltaSeconds();
	this->AddControllerPitchInput(Rate * this->BaseLookUpRate * SecondsPerFrame);
}

void AStarRunner2019Character::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AHallwayActor::StaticClass()))
	{
		auto* HallwayActor = Cast<AHallwayActor>(OtherActor);
		auto* HallwayTriggerBox = HallwayActor->GetHallwayJointComponent()->GetTriggerBox();

		if (HallwayTriggerBox == OtherComp)
		{
			++this->HallwaysPassedCount;
			
			const bool bAtSpeedStep = this->HallwaysPassedCount % 5 == 0;
			const bool bAtMaxSpeed = this->MovementComponent->MaxWalkSpeed >= MAX_SPEED;
			if (bAtSpeedStep && !bAtMaxSpeed)
			{
				this->MovementComponent->MaxWalkSpeed += SPEED_STEP;

				AStarRunner2019HUD* StarRunnerHUD
					= Cast<AStarRunner2019HUD>(this->GetWorld()->GetFirstPlayerController()->GetHUD());

				float SpeedPercentage = this->MovementComponent->MaxWalkSpeed / MAX_SPEED;
				StarRunnerHUD->SetSpeedBar(SpeedPercentage);

			}
		}
	}
}

void AStarRunner2019Character::Turn(EDirection Direction)
{
	if (this->bIsTurnable) {
		this->TargetRotation = FRotator(this->GetActorRotation());
		this->TargetRotation.Yaw += static_cast<float>(Direction) * 90;

		this->bIsTurning = true;
		this->bIsTurnable = false;
		this->TurnDirection = Direction;
	}
}