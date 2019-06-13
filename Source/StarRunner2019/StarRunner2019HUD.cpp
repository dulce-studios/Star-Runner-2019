// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StarRunner2019HUD.h"

#include "Kismet/GameplayStatics.h"
#include "Misc/Timespan.h"
#include "StarRunner2019Character.h"
#include "UObject/ConstructorHelpers.h"

AStarRunner2019HUD::AStarRunner2019HUD() 
{
	static ConstructorHelpers::FClassFinder<UStarRunner2019Widget> StarRunnerWidgetClassFinder(
		TEXT("/Game/Blueprints/StarRunner2019Widget"));

	static ConstructorHelpers::FClassFinder<UPauseWidget> PauseWidgetClassFinder(
		TEXT("/Game/Blueprints/PauseWidget"));

	this->StarRunnerWidget = CreateWidget<UStarRunner2019Widget>(
		this->GetWorld(),
		StarRunnerWidgetClassFinder.Class);

	this->PauseWidget = CreateWidget<UPauseWidget>(
		this->GetWorld(),
		PauseWidgetClassFinder.Class);
}

void AStarRunner2019HUD::BeginPlay() 
{
	UProgressBar* SpeedBar = this->StarRunnerWidget->SpeedBar;
	this->StarRunnerWidget->AddToViewport(0);

	UButton* PauseContinueButton = this->PauseWidget->ContinueButton;
	PauseContinueButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::PauseMenuContinueClicked);

	UButton* PauseRestartButton = this->PauseWidget->RestartButton;
	PauseRestartButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::PauseMenuRestartClicked);

	UButton* PauseQuitButton = this->PauseWidget->QuitButton;
	PauseQuitButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::PauseMenuQuitClicked);
}

void AStarRunner2019HUD::SetSpeedBar(float SpeedPercentage)
{
	this->StarRunnerWidget->SpeedBar->SetPercent(SpeedPercentage);
}

void AStarRunner2019HUD::SetElapsedTime(float ElapsedSeconds)
{
	FTimespan Timespan = FTimespan::FromSeconds(ElapsedSeconds);
	FText TimeSpanText = FText::AsTimespan(Timespan);
	this->StarRunnerWidget->ElapsedTime->SetText(TimeSpanText);
}

void AStarRunner2019HUD::ShowPauseMenu()
{
	this->PauseWidget->AddToViewport(1);
	this->PlayerOwner->bShowMouseCursor = true;
}

void AStarRunner2019HUD::ClosePauseMenu()
{
	this->PlayerOwner->bShowMouseCursor = false;
	this->PauseWidget->RemoveFromViewport();
}

void AStarRunner2019HUD::PauseMenuContinueClicked()
{
	auto* PlayerCharacter = Cast<AStarRunner2019Character>(this->PlayerOwner->GetCharacter());
	PlayerCharacter->TogglePaused();
}

void AStarRunner2019HUD::PauseMenuRestartClicked()
{
	this->PlayerOwner->RestartLevel();
}

void AStarRunner2019HUD::PauseMenuQuitClicked()
{
	const bool bIgnorePlatformRestrictions = false;
	UKismetSystemLibrary::QuitGame(
		this->GetWorld(),
		this->PlayerOwner,
		EQuitPreference::Quit,
		bIgnorePlatformRestrictions);
}
