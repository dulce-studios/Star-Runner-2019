// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HallwayActor.generated.h"

UCLASS()
class STARRUNNER2019_API AHallwayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHallwayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Setup();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnLeftChildHallway();
	void SpawnRightChildHallway();

};
