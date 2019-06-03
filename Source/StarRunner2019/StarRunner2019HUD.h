// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "StartMenuWidget.h"

#include "StarRunner2019HUD.generated.h"

/**
 * 
 */
UCLASS()
class STARRUNNER2019_API AStarRunner2019HUD : public AHUD
{
	GENERATED_BODY()

public:
	AStarRunner2019HUD();

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartButtonClicked();
private:
	UPROPERTY()
	UStartMenuWidget* StartMenu;
};
