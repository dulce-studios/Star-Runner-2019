// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
private:
	UPROPERTY()
	UStartMenuWidget* StartMenu;
};
