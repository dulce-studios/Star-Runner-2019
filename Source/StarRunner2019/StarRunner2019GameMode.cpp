// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019GameMode.h"
#include "StarRunner2019HUD.h"

AStarRunner2019GameMode::AStarRunner2019GameMode()
{
	this->DefaultPawnClass = AStarRunner2019Character::StaticClass();
	this->HUDClass = AStarRunner2019HUD::StaticClass();
}

AHallwayActor* AStarRunner2019GameMode::SpawnInitialHallway(FTransform& spawnTransform)
{
	UWorld* World = this->GetWorld();
	if (!World) {
		return nullptr;
	}

	FActorSpawnParameters spawnParams;
	return World->SpawnActor<AHallwayActor>(
		AHallwayActor::StaticClass(),
		spawnTransform,
		spawnParams);
}

void AStarRunner2019GameMode::BeginPlay()
{
	// Based on https://answers.unrealengine.com/questions/337711/getworldtransform-for-scenecomponent-in-c.html
	AController* PlayerController = this->DefaultPawnClass.GetDefaultObject()->GetController();
	AActor* PlayerStart = this->FindPlayerStart(PlayerController, FString());
	FTransform SpawnTransform(PlayerStart->GetRootComponent()->GetComponentTransform());
	SpawnTransform.AddToTranslation(FVector(0, 0, -120));

	AHallwayActor* InitialHallway = this->SpawnInitialHallway(SpawnTransform);
	InitialHallway->SpawnLeftChildHallway();
	InitialHallway->SpawnRightChildHallway();
}
