// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StarRunner2019HUD.h"

#include "Kismet/GameplayStatics.h"
#include "Misc/Timespan.h"
#include "GameFramework/PlayerController.h"
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
	this->PlayerOwner->SetInputMode(FInputModeGameOnly());
	UProgressBar* SpeedBar = this->StarRunnerWidget->SpeedBar;
	this->StarRunnerWidget->AddToViewport(0);

	UButton* PauseContinueButton = this->PauseWidget->ContinueButton;
	PauseContinueButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::PauseMenuContinueClicked);

	UButton* PauseRestartButton = this->PauseWidget->RestartButton;
	PauseRestartButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::RestartGame);

	UButton* PauseQuitButton = this->PauseWidget->QuitButton;
	PauseQuitButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::QuitGame);

	UButton* GameOverRestartButton = this->GameOverWidget->RestartButton;
	GameOverRestartButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::RestartGame);

	UButton* GameOverQuitButton = this->GameOverWidget->QuitButton;
	GameOverQuitButton->OnClicked.AddDynamic(
		this, &AStarRunner2019HUD::QuitGame);
}

void AStarRunner2019HUD::SetSpeedBar(float SpeedPercentage)
{
	this->StarRunnerWidget->SpeedBar->SetPercent(SpeedPercentage);
}

void AStarRunner2019HUD::SetElapsedTime(float ElapsedSeconds)
{
	const FText TimeSpanText = this->ConvertSecondsToText(ElapsedSeconds);
	this->StarRunnerWidget->ElapsedTime->SetText(TimeSpanText);
}

void AStarRunner2019HUD::ShowPauseMenu()
{
	this->ShowMenu(this->PauseWidget);
}

void AStarRunner2019HUD::ClosePauseMenu()
{
	this->PlayerOwner->bShowMouseCursor = false;
	this->PauseWidget->RemoveFromViewport();
	this->PlayerOwner->SetInputMode(FInputModeGameOnly());
}

void AStarRunner2019HUD::PauseMenuContinueClicked()
{
	auto* PlayerCharacter = Cast<AStarRunner2019Character>(this->PlayerOwner->GetCharacter());
	PlayerCharacter->TogglePaused();
}

void AStarRunner2019HUD::ShowGameOverMenu()
{
	this->ShowMenu(this->GameOverWidget);
}

void AStarRunner2019HUD::ShowMenu(UUserWidget* Widget)
{
	Widget->AddToViewport(1);
	this->PlayerOwner->bShowMouseCursor = true;
	FInputModeUIOnly FocusMenuMode;
	FocusMenuMode.SetWidgetToFocus(Widget->TakeWidget());
	this->PlayerOwner->SetInputMode(FocusMenuMode);
}

void AStarRunner2019HUD::SetGameOverHallwaysPassedText(unsigned int hallwaysPassedCount)
{
	this->FormatTextBlock(this->GameOverWidget->HallwaysPassedText, hallwaysPassedCount);
}

void AStarRunner2019HUD::SetGameOverTimeElapsedText(float elapsedSeconds)
{
	const FText TimeSpanText = this->ConvertSecondsToText(elapsedSeconds);
	this->FormatTextBlock(this->GameOverWidget->TimeElapsedText, TimeSpanText);
}

void AStarRunner2019HUD::RestartGame()
{
	this->PlayerOwner->RestartLevel();
}

void AStarRunner2019HUD::QuitGame()
{
	FName NextLevelName = TEXT("StartMenu");
	UGameplayStatics::OpenLevel(
		this->GetWorld(),
		NextLevelName);
}

template <typename T>
void AStarRunner2019HUD::FormatTextBlock(UTextBlock* textBlock, T value)
{
	FText PlaceholderText = textBlock->GetText();

	FFormatNamedArguments Args;
	Args.Add("TextPlaceholder", PlaceholderText);
	Args.Add("Value", value);

	FText text = FText::Format(
		NSLOCTEXT(
			"StarRunner2019",
			"StarRunner2019HUD",
			"{TextPlaceholder} {Value}"),
		Args);

	textBlock->SetText(text);
}

FText AStarRunner2019HUD::ConvertSecondsToText(float seconds)
{
	FTimespan Timespan = FTimespan::FromSeconds(seconds);
	return FText::AsTimespan(Timespan);
}

