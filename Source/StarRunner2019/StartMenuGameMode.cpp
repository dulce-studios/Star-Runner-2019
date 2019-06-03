// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuGameMode.h"
#include "StarRunner2019HUD.h"

AStartMenuGameMode::AStartMenuGameMode() {
	this->HUDClass = AStarRunner2019HUD::StaticClass();
}