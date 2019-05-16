// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019GameMode.h"
#include "StarRunner2019Character.h"
#include "UObject/ConstructorHelpers.h"

AStarRunner2019GameMode::AStarRunner2019GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Content/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

// AHallwayUnitActor* AStarRunner2019GameMode::SpawnInitialHallway()
// {
	// AActor* PlayerStart = FindPlayerStart(NULL, NULL);

	// USceneComponent* RootComponent = PlayerStart->GetRootComponent();

	// Based on https://answers.unrealengine.com/questions/337711/getworldtransform-for-scenecomponent-in-c.html
	// FTransform WorldTransform = RootComponent->GetComponentTransform();
	// FVector SpawnTranslationOffset(0, 0, -120);

	// FVector SpawnTranslation = WorldTransform.GetTranslation() + SpawnTranslationOffset;
	// FQuat SpawnRotation = WorldTransform.GetRotation();
	// FVector SpawnScale(50, 50, 50);

	// FTransform SpawnTransform(SpawnRotation, SpawnTranslation, SpawnScale);

	// AHallwayUnitActor* Boi = GetWorld()->SpawnActor<AHallwayUnitActor>(GetClass(), SpawnTransform);

// 	UE_LOG(LogTemp, Warning, TEXT("WEEE"));


// 	return NULL;
// }

// void AStarRunner2019GameMode::BeginPlay()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Game Mode BeginPlay bruh"));

// 	SpawnInitialHallway();
// }
