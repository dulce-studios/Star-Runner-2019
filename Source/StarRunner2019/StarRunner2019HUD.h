// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "StarRunner2019Widget.h"
#include "GameOverWidget.h"
#include "PauseWidget.h"

#include "GameFramework/HUD.h"
#include "StarRunner2019HUD.generated.h"

UCLASS()
class STARRUNNER2019_API AStarRunner2019HUD : public AHUD
{
	GENERATED_BODY()

public:
	AStarRunner2019HUD();

	UFUNCTION()
	virtual void BeginPlay() override;

	void SetSpeedBar(float SpeedPercentage);
	void SetElapsedTime(float ElapsedSeconds);
	void ShowPauseMenu();
	void ClosePauseMenu();
	void ShowGameOverMenu();
	void RestartGame();
	void QuitGame();

	void SetGameOverHallwaysPassedText(unsigned int hallwaysPassedCount);
	void SetGameOverTimeElapsedText(float elapsedSeconds);

	template <typename T>
	void FormatTextBlock(UTextBlock* textBlock, T value);

	FText ConvertFloatSecondsToText(float seconds);

	UFUNCTION()
	void PauseMenuContinueClicked();

	UFUNCTION()
	void PauseMenuRestartClicked();

	UFUNCTION()
	void PauseMenuQuitClicked();

	UFUNCTION()
	void GameOverRestartClicked();

	UFUNCTION()
	void GameOverQuitClicked();

private:
	UPROPERTY()
	UStarRunner2019Widget* StarRunnerWidget;

	UPROPERTY()
	UPauseWidget* PauseWidget;

	UPROPERTY()
	UGameOverWidget* GameOverWidget;
};
