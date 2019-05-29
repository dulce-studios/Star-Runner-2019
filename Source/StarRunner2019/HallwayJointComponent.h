// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Engine/Classes/Components/ArrowComponent.h"
#include "HallwayJointComponent.generated.h"

UCLASS()
class STARRUNNER2019_API UHallwayJointComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
    UHallwayJointComponent();

private:
	UFUNCTION()
	void SetupTriggerBox();

	UFUNCTION()
	void SetupLeftArrowComponent();

	UFUNCTION()
	void SetupRightArrowComponent();

	void AttachComponentToSelf(UPrimitiveComponent* const component, const FTransform transform);
};