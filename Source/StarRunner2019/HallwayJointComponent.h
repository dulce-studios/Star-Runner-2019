// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "HallwayJointComponent.generated.h"

UCLASS()
class STARRUNNER2019_API UHallwayJointComponent
	: public UStaticMeshComponent {
    GENERATED_BODY()

public:
    UHallwayJointComponent();

	UFUNCTION()
	UArrowComponent* GetLeftArrow();

	UFUNCTION()
	UArrowComponent* GetRightArrow();

	UFUNCTION()
	UBoxComponent* GetTriggerBox();

private:
	UPROPERTY()
	UArrowComponent* leftArrow;

	UPROPERTY()
	UArrowComponent* rightArrow;

	UPROPERTY()
	UBoxComponent* triggerBox;

	template<class C>
	void AttachWithTransform(
		FName componentName,
		FTransform& componentTransform);
};
