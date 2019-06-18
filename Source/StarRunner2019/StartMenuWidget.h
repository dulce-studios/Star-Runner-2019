// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuWidget.generated.h"

UCLASS()
class STARRUNNER2019_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CreditsButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitGameButton;
};
