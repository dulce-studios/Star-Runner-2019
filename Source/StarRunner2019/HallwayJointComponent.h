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
	UBoxComponent* GetHallwaySpawnManagerBox();

	UFUNCTION()
	UBoxComponent* GetHallwayGameOverBox();

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UBoxComponent* hallwaySpawnManagerBox;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UBoxComponent* hallwayGameOverBox;

	UBoxComponent* AttachChildComponent(FName ChildComponentName, FTransform ChildComponentTransform);
};
