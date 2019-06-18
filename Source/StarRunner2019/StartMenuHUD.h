// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CreditsWidget.h"
#include "StartMenuWidget.h"
#include "GameFramework/HUD.h"
#include "StartMenuHUD.generated.h"

UCLASS()
class STARRUNNER2019_API AStartMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	AStartMenuHUD();

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void CreditsButtonClicked();

	UFUNCTION()
	void ExitButtonClicked();

	UFUNCTION()
	void BackButtonClicked();

private:
	UPROPERTY()
	UStartMenuWidget* StartMenu;

	UPROPERTY()
	UCreditsWidget* Credits;
};
