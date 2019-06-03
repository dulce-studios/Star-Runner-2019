// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HallwayActor.h"
#include "StarRunner2019Character.h"

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "StarRunner2019GameMode.generated.h"

UCLASS(minimalapi)
class AStarRunner2019GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStarRunner2019GameMode();

protected:
 	virtual void BeginPlay();

private:
	AHallwayActor* SpawnInitialHallway(FTransform& spawnTransform);

};
