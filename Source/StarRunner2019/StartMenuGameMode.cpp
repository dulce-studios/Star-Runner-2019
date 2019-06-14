// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StartMenuGameMode.h"
#include "StartMenuHUD.h"

AStartMenuGameMode::AStartMenuGameMode() 
{
	this->HUDClass = AStartMenuHUD::StaticClass();
}