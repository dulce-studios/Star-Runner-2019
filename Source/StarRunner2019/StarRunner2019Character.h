// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "StarRunner2019HUD.h"
#include "StarRunner2019Character.generated.h"

class UInputComponent;

UENUM()
enum class EDirection : int8 {
	None = 0,
	Left = -1,
	Right = 1,
};

UCLASS(config = Game)
class AStarRunner2019Character : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FirstPersonCameraComponent;

public:
	AStarRunner2019Character();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly)
	float MoveSpeedRatio;

	UPROPERTY(BlueprintReadOnly)
	float GameTime;

	UPROPERTY(BlueprintReadOnly)
	float AccelerationRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTurnable;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTurning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDirection TurnDirection;

	unsigned int HallwaysPassedCount;

	unsigned int NextSpeedupThreshold;

	unsigned int NumSpeedups;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadWrite)
	UCapsuleComponent* CharacterCapsuleComponent;

	FRotator TargetRotation;

	FTimerHandle GameClock;

	AStarRunner2019HUD* PlayerHUD;
private:
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void UpdateGameClock();

	void Turn(EDirection Direction);

	void SpeedUp();

protected:
	void MoveForward(float val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void TurnLeft();

	void TurnRight();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
