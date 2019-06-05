// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StarRunner2019HUD.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AStarRunner2019HUD::AStarRunner2019HUD() 
{
	static ConstructorHelpers::FClassFinder<UStarRunner2019Widget> StarRunnerWidget(
		TEXT("/Game/Blueprints/StarRunner2019Widget"));

	this->StarRunnerWidget = CreateWidget<UStarRunner2019Widget>(
		this->GetWorld(),
		StarRunnerWidget.Class);
}

void AStarRunner2019HUD::BeginPlay() 
{
	UProgressBar* SpeedBar = this->StarRunnerWidget->SpeedBar;
	this->StarRunnerWidget->AddToViewport();
}

void AStarRunner2019HUD::SetSpeedBar(float SpeedPercentage)
{
	this->StarRunnerWidget->SpeedBar->SetPercent(SpeedPercentage);
}