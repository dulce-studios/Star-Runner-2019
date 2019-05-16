// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HallwayUnitActor.h"
#include "StarRunner2019GameMode.generated.h"

UCLASS(minimalapi)
class AStarRunner2019GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStarRunner2019GameMode();

	// AHallwayUnitActor* SpawnInitialHallway();

// protected:
// 	virtual void BeginPlay() override;
};
