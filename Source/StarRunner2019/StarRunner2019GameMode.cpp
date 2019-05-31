// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019GameMode.h"

AStarRunner2019GameMode::AStarRunner2019GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/Blueprints/FirstPersonCharacter"));
	this->DefaultPawnClass = PlayerPawnClassFinder.Class;
}

AHallwayActor* AStarRunner2019GameMode::SpawnInitialHallway()
{
	UWorld* World = this->GetWorld();
	if (!World) {
		return nullptr;
	}

	AController* playerController = this->DefaultPawnClass.GetDefaultObject()->GetController();
	AActor* playerStart = this->FindPlayerStart(playerController, FString());

	// Based on https://answers.unrealengine.com/questions/337711/getworldtransform-for-scenecomponent-in-c.html
	FTransform spawnTransform(playerStart->GetRootComponent()->GetComponentTransform());
	spawnTransform.AddToTranslation(FVector(0, 0, -120));
	spawnTransform.SetScale3D(FVector(50));

	FActorSpawnParameters spawnParams;
	AHallwayActor* Boi = World->SpawnActor<AHallwayActor>(
		AHallwayActor::StaticClass(),
		spawnTransform,
		spawnParams);
	return Boi;
 }

 void AStarRunner2019GameMode::BeginPlay()
 {
 	AHallwayActor* InitialHallway = SpawnInitialHallway();
	InitialHallway->SpawnLeftChildHallway();
	InitialHallway->SpawnRightChildHallway();
 }
