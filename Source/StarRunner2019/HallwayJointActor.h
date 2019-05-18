// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "HallwayJointActor.generated.h"

UCLASS()
class STARRUNNER2019_API AHallwayJointActor : public AStaticMeshActor
{
    GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AHallwayJointActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};