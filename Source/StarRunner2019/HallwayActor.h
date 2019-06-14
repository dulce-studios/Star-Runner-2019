// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "HallwayJointComponent.h"

#include "HallwayActor.generated.h"

UCLASS()
class STARRUNNER2019_API AHallwayActor : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHallwayActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnLeftChildHallway();

	UFUNCTION()
	void SpawnRightChildHallway();

	UHallwayJointComponent* GetHallwayJointComponent();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UHallwayJointComponent* HallwayJointComponent;

	UPROPERTY(BlueprintReadOnly)
	AHallwayActor* LeftChildHallway;

	UPROPERTY(BlueprintReadOnly)
	AHallwayActor* RightChildHallway;

private:
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlapComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	AHallwayActor* SpawnHallFromYawAndOffset(
		float yawDegrees);
};
