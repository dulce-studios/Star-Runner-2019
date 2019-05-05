// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StarRunner2019GameMode.h"
#include "StarRunner2019Character.h"
#include "UObject/ConstructorHelpers.h"

AStarRunner2019GameMode::AStarRunner2019GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
