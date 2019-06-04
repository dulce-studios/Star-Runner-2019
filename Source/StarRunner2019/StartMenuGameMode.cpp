// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StartMenuGameMode.h"
#include "StarRunner2019HUD.h"

AStartMenuGameMode::AStartMenuGameMode() 
{
	this->HUDClass = AStarRunner2019HUD::StaticClass();
}