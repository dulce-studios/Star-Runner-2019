// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

/**
 * 
 */
UCLASS()
class STARRUNNER2019_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;
};
