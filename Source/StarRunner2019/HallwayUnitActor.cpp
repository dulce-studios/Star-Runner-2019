// Copyright 1998-2019 Dulce Studios. All Rights Reserved.

#include "HallwayUnitActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AHallwayUnitActor::AHallwayUnitActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// // Based on https://forums.unrealengine.com/development-discussion/c-gameplay-programming/118457-set-static-mesh-in-c
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayunit.hallwayunit'"));
	UStaticMesh *StaticMesh = MeshAsset.Object;
	
	MeshComponent->SetStaticMesh(StaticMesh);
	// Assign the static mesh component to root
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AHallwayUnitActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHallwayUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
