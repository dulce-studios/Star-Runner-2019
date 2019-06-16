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
#include "Public/TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//TODO Use Enum to model left/right turn for this and hallways

constexpr float START_SPEED_RATIO = 0.2;
constexpr unsigned int TOTALSPEEDUPS = 10;

//////////////////////////////////////////////////////////////////////////
// AStarRunner2019Character

AStarRunner2019Character::AStarRunner2019Character()
{
	// Set size for collision capsule
	this->CharacterCapsuleComponent = GetCapsuleComponent();
	this->CharacterCapsuleComponent->InitCapsuleSize(55.f, 96.0f);

	this->bIsPaused = false;

	this->bIsTurnable = false;
	this->HallwaysPassedCount = 0;
	this->NextSpeedupThreshold = 5;
	this->NumSpeedups = 1;
	this->GameTime = 0;

	this->bIsTurning = false;
	this->TurnDirection = EDirection::None;
	this->GetCharacterMovement()->MaxWalkSpeed = 1500;
	this->MoveSpeedRatio = START_SPEED_RATIO;
	this->AccelerationRatio = 0.08;

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

	this->PlayerHUD = Cast<AStarRunner2019HUD>(
		this->GetWorld()->GetFirstPlayerController()->GetHUD());
	this->PlayerHUD->SetSpeedBar(static_cast<float>(1) / TOTALSPEEDUPS);

	// Set Clock Timer
	const float RateSeconds = 1;
	const bool bDoLoop = true;
	const float FirstDelay = 0;
	this->GetWorldTimerManager().SetTimer(
		this->GameClock,
		this,
		&AStarRunner2019Character::UpdateGameClock,
		RateSeconds,
		bDoLoop,
		FirstDelay);

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

	FInputActionBinding& toggle = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AStarRunner2019Character::TogglePaused);
	toggle.bExecuteWhenPaused = true; // catches input when paused

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AStarRunner2019Character::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AStarRunner2019Character::LookUpAtRate);

	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AStarRunner2019Character::TurnLeft);
	PlayerInputComponent->BindAction("TurnLeft", IE_Released, this, &AStarRunner2019Character::TurnLeft);

	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AStarRunner2019Character::TurnRight);
	PlayerInputComponent->BindAction("TurnRight", IE_Released, this, &AStarRunner2019Character::TurnRight);
}

void AStarRunner2019Character::TurnLeft() 
{
	this->Turn(EDirection::Left);
}

void AStarRunner2019Character::TurnRight() 
{
	this->Turn(EDirection::Right);
}

void AStarRunner2019Character::Tick(float DeltaSeconds)
{
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

void AStarRunner2019Character::TogglePaused()
{
	this->bIsPaused = !this->bIsPaused;
	if (this->bIsPaused) {
		this->PlayerHUD->ShowPauseMenu();
	}
	else {
		this->PlayerHUD->ClosePauseMenu();
	}
	UGameplayStatics::SetGamePaused(this->GetWorld(), this->bIsPaused);
}

void AStarRunner2019Character::MoveForward(float val)
{
	this->AddMovementInput(this->GetActorForwardVector(), this->MoveSpeedRatio);
}

void AStarRunner2019Character::MoveRight(float Value)
{
	if (Value != 0) {
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

void AStarRunner2019Character::SpeedUp()
{
	const bool bAtSpeedStep = this->HallwaysPassedCount == this->NextSpeedupThreshold;
	const bool bAtMaxSpeed = this->NumSpeedups == TOTALSPEEDUPS;
	if (bAtSpeedStep && !bAtMaxSpeed) {
		this->HallwaysPassedCount = 0;
		this->NextSpeedupThreshold++;

		this->MoveSpeedRatio += this->AccelerationRatio;
		this->MoveSpeedRatio = FMath::Clamp<float>(this->MoveSpeedRatio, 0, 1);

		const float SpeedBarRatio = static_cast<float>(++this->NumSpeedups) / TOTALSPEEDUPS;
		this->PlayerHUD->SetSpeedBar(SpeedBarRatio);
		this->AddMovementInput(this->GetActorForwardVector(), this->MoveSpeedRatio);
	}
}

void AStarRunner2019Character::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AHallwayActor::StaticClass())) {
		auto* HallwayActor = Cast<AHallwayActor>(OtherActor);
		UBoxComponent* spawnManagerBox = HallwayActor->GetHallwayJointComponent()->GetHallwaySpawnManagerBox();

		if (spawnManagerBox == OtherComp) {
			++this->HallwaysPassedCount;
			this->SpeedUp();
		}
	}
}

void AStarRunner2019Character::UpdateGameClock()
{
	this->PlayerHUD->SetElapsedTime(this->GameTime++);
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