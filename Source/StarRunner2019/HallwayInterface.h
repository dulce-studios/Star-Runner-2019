// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"
#include "Math/TransformNonVectorized.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UObject/Interface.h"

#include "HallwayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHallwayInterface : public UInterface
{
	GENERATED_BODY()
};

class STARRUNNER2019_API IHallwayInterface
{
	GENERATED_BODY()

public:
	virtual FTransform* GetTransformForComponent(USceneComponent* Component);
	virtual void AttachComponentToAnotherComponent(USceneComponent* Component, USceneComponent* OtherComponent, FTransform ComponentTransform);
	virtual AActor* SpawnChildActor(AActor* ParentActor, FTransform* Transform);
};
