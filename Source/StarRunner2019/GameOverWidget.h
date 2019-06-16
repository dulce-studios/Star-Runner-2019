// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "GameOverWidget.generated.h"

UCLASS()
class STARRUNNER2019_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HallwaysPassedText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TimeElapsedText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QuitButton;
};
