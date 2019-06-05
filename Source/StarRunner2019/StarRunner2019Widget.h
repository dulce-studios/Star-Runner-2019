// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "StarRunner2019Widget.generated.h"

UCLASS()
class STARRUNNER2019_API UStarRunner2019Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* SpeedBar;
};
