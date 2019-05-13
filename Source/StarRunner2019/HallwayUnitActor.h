// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#include "HallwayUnitActor.generated.h"

UCLASS()
class STARRUNNER2019_API AHallwayUnitActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHallwayUnitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY()
	USceneComponent *Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;
};
