// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Engine/Classes/Components/ArrowComponent.h"
#include "HallwayJointComponent.generated.h"

UCLASS(Blueprintable)
class STARRUNNER2019_API UHallwayJointComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
    UHallwayJointComponent();

private:
	UFUNCTION(BlueprintCallable)
	void SetupTriggerBox();

	UFUNCTION(BlueprintCallable)
	void SetupLeftArrowComponent();

	UFUNCTION(BlueprintCallable)
	void SetupRightArrowComponent();
};