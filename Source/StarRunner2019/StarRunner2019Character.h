// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StarRunner2019Character.generated.h"

class UInputComponent;

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
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTurnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WentLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HallwaysPassedCount;

	UPROPERTY(BlueprintReadWrite)
	UCharacterMovementComponent* MovementComponent;
	

private:
	void SetupCharacterMovementComponent();

protected:
	/** Handles moving forward/backward */
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
	FORCEINLINE class UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
