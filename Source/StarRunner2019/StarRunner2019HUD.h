// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "StarRunner2019Widget.h"
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

	UFUNCTION()
	void PauseMenuContinueClicked();

	UFUNCTION()
	void PauseMenuRestartClicked();

	UFUNCTION()
	void PauseMenuQuitClicked();

private:
	UPROPERTY()
	UStarRunner2019Widget* StarRunnerWidget;

	UPROPERTY()
	UPauseWidget* PauseWidget;
};
