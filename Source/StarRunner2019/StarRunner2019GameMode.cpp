// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019GameMode.h"
#include "StarRunner2019Character.h"
#include "UObject/ConstructorHelpers.h"

AStarRunner2019GameMode::AStarRunner2019GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

AHallwayActor* AStarRunner2019GameMode::SpawnInitialHallway()
{
	AController* PlayerPawnController = DefaultPawnClass.GetDefaultObject()->GetController();
	AActor* PlayerStartActor = FindPlayerStart(PlayerPawnController, FString());

	UE_LOG(LogTemp, Warning, TEXT("ZORG"));



	USceneComponent* RootComponent = PlayerStartActor->GetRootComponent();

	UE_LOG(LogTemp, Warning, TEXT("Sad %d"), RootComponent == NULL);

	// Based on https://answers.unrealengine.com/questions/337711/getworldtransform-for-scenecomponent-in-c.html
	FTransform WorldTransform = RootComponent->GetComponentTransform();
	
	FVector SpawnTranslationOffset(0, 0, -120);
	
	FVector SpawnTranslation = WorldTransform.GetTranslation() + SpawnTranslationOffset;
	FQuat SpawnRotation = WorldTransform.GetRotation();
	FVector SpawnScale(50, 50, 50);

	FTransform SpawnTransform(SpawnRotation, SpawnTranslation, SpawnScale);

	FActorSpawnParameters Info;

	// Notice https://stackoverflow.com/questions/5503352/const-before-or-const-after
	UWorld* const World = GetWorld();
	UE_LOG(LogTemp, Warning, TEXT("Heh %d"), World == NULL);
	if (World) {
		AHallwayActor* Boi = World->SpawnActor<AHallwayActor>(AHallwayActor::StaticClass(), SpawnTransform, Info);
		return Boi;
	}
	else {
		return NULL;
	}
 }

 void AStarRunner2019GameMode::BeginPlay()
 {
 	SpawnInitialHallway();
 }
