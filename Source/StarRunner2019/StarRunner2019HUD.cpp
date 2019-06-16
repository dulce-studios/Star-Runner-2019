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

	static ConstructorHelpers::FClassFinder<UGameOverWidget> GameOverWidgetClassFinder(
		TEXT("/Game/Blueprints/GameOverWidget"));

	this->StarRunnerWidget = CreateWidget<UStarRunner2019Widget>(
		this->GetWorld(),
		StarRunnerWidgetClassFinder.Class);

	this->PauseWidget = CreateWidget<UPauseWidget>(
		this->GetWorld(),
		PauseWidgetClassFinder.Class);

	this->GameOverWidget = CreateWidget<UGameOverWidget>(
		this->GetWorld(),
		GameOverWidgetClassFinder.Class);
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

	UButton* GameOverRestartButton = this->GameOverWidget->RestartButton;
	GameOverRestartButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::GameOverRestartClicked);

	UButton* GameOverQuitButton = this->GameOverWidget->QuitButton;
	GameOverQuitButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::GameOverQuitClicked);
}

void AStarRunner2019HUD::SetSpeedBar(float SpeedPercentage)
{
	this->StarRunnerWidget->SpeedBar->SetPercent(SpeedPercentage);
}

void AStarRunner2019HUD::SetElapsedTime(float ElapsedSeconds)
{
	FText TimeSpanText = ConvertFloatSecondsToText(ElapsedSeconds);
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
	this->RestartGame();
}

void AStarRunner2019HUD::PauseMenuQuitClicked()
{
	this->QuitGame();
}

void AStarRunner2019HUD::ShowGameOverMenu()
{
	this->GameOverWidget->AddToViewport(1);
	this->PlayerOwner->bShowMouseCursor = true;
}

void AStarRunner2019HUD::GameOverRestartClicked()
{
	this->RestartGame();
}

void AStarRunner2019HUD::GameOverQuitClicked()
{
	this->QuitGame();
}


void AStarRunner2019HUD::SetGameOverHallwaysPassedText(unsigned int hallwaysPassedCount)
{
	this->FormatTextBlock(this->GameOverWidget->HallwaysPassedText, hallwaysPassedCount);
}

void AStarRunner2019HUD::SetGameOverTimeElapsedText(float elapsedSeconds)
{
	FText timeSpanText = ConvertFloatSecondsToText(elapsedSeconds);
	this->FormatTextBlock(this->GameOverWidget->TimeElapsedText, timeSpanText);
}

void AStarRunner2019HUD::RestartGame()
{
	this->PlayerOwner->RestartLevel();
}

void AStarRunner2019HUD::QuitGame()
{
	const bool bIgnorePlatformRestrictions = false;
	UKismetSystemLibrary::QuitGame(
		this->GetWorld(),
		this->PlayerOwner,
		EQuitPreference::Quit,
		bIgnorePlatformRestrictions);
}

template <typename T>
void AStarRunner2019HUD::FormatTextBlock(UTextBlock* textBlock, T value)
{
	FText placeholderText = textBlock->GetText();

	FFormatNamedArguments args;
	args.Add("TextPlaceholder", placeholderText);
	args.Add("Value", value);

	FText text = FText::Format(
		NSLOCTEXT(
			"StarRunner2019",
			"StarRunner2019HUD",
			"{TextPlaceholder} {Value}"),
		args);

	textBlock->SetText(text);
}

FText AStarRunner2019HUD::ConvertFloatSecondsToText(float seconds)
{
	FTimespan Timespan = FTimespan::FromSeconds(seconds);
	return FText::AsTimespan(Timespan);
}

