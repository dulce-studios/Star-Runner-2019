// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StarRunner2019HUD.h"

#include "Kismet/GameplayStatics.h"
#include "Misc/Timespan.h"
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

void AStarRunner2019HUD::SetElapsedTime(float ElapsedSeconds)
{
	FTimespan Timespan = FTimespan::FromSeconds(ElapsedSeconds);

	int32 MinutesElapsed = Timespan.GetMinutes();
	int32 SecondsElapsed = Timespan.GetSeconds();
	
	// Based on https://answers.unrealengine.com/questions/41383/how-concatenate-ftext-together.html
	FFormatNamedArguments Args;
	Args.Add("MinutesElapsed", MinutesElapsed);
	Args.Add("SecondsElapsed", SecondsElapsed);

	// See https://docs.unrealengine.com/en-US/Gameplay/Localization/Formatting/index.html
	FText TimeElapsed = FText::Format(NSLOCTEXT("StarRunner2019HUD", "TimeElapsed", "{MinutesElapsed} : {SecondsElapsed}"), Args);
	this->StarRunnerWidget->ElapsedTime->SetText(TimeElapsed);
}