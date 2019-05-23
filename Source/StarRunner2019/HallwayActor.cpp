// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayActor.h"

#include "Engine/Classes/Kismet/KismetSystemLibrary.h"

// For constructing the hallways
#include "HallwayJointComponent.h"
#include "HallwayUnitComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <stdlib.h>

// Sets default values
AHallwayActor::AHallwayActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent *RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	Setup();
}

void AHallwayActor::Setup()
{
	UHallwayUnitComponent *InitialHallwayUnitComponent = CreateDefaultSubobject<UHallwayUnitComponent>(TEXT("InitialHallwayUnitComponent"));
	InitialHallwayUnitComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(InitialHallwayUnitComponent, Origin, BoxExtent, SphereRadius);

	float XLength = (BoxExtent.X * 2.0f);

	FRotator Rotator(0.0f, 0.0f, 0.0f);
	FVector Translation(XLength, 0.0f, 0.0f);
	FVector Scale(1.0f, 1.0f, 1.0f);

	int HallwayCount = rand() % 5 + 1;

	UE_LOG(LogTemp, Warning, TEXT("Generating %d hallways"), HallwayCount);

	int i;
	for (i = 1; i < HallwayCount; ++i)
	{
		UHallwayUnitComponent *NextHallwayUnitComponent = CreateDefaultSubobject<UHallwayUnitComponent>(FName(*FString::Printf(TEXT("HallwayUnitComponent%d"), i)));
		FTransform HallwayUnitComponentTransform(Rotator, Translation, Scale);

		NextHallwayUnitComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		NextHallwayUnitComponent->SetRelativeTransform(HallwayUnitComponentTransform);

		Translation.X += XLength;
	}

	UHallwayJointComponent *HallwayJointComponent = CreateDefaultSubobject<UHallwayJointComponent>(TEXT("HallwayJointComponent"));
	FTransform HallwayJointComponentTransform(Rotator, Translation, Scale);

	HallwayJointComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	HallwayJointComponent->SetRelativeTransform(HallwayJointComponentTransform);
	
}

void AHallwayActor::SpawnLeftChildHallway() {
	UHallwayJointComponent* const HallwayJointComponent = Cast<UHallwayJointComponent>(GetDefaultSubobjectByName(TEXT("HallwayJointComponent")));
	const UArrowComponent* LeftArrowComponent = Cast<UArrowComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("LeftArrowComponent")));

	FTransform LeftArrowComponentTransform = LeftArrowComponent->GetComponentTransform();

	FVector LeftArrowComponentLocation = LeftArrowComponentTransform.GetLocation();
	FQuat LeftArrowComponentRotation = LeftArrowComponentTransform.GetRotation();

	FVector VectorOffset(250, 0, 0);
	FVector RotationVector = UKismetMathLibrary::Quat_RotateVector(LeftArrowComponentRotation, VectorOffset);

	FVector LeftChildHallwayLocation = LeftArrowComponentLocation + RotationVector;
	FQuat LeftChildHallwayRotation = LeftArrowComponentRotation;
	FVector LeftChildHallwayScale = GetActorScale3D();

	FTransform LeftChildHallwayTransform(LeftChildHallwayRotation, LeftChildHallwayLocation, LeftChildHallwayScale);

	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters Info;
		AHallwayActor* LeftChildHallway = World->SpawnActor<AHallwayActor>(AHallwayActor::StaticClass(), LeftChildHallwayTransform, Info);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Nooose"));
	}
}

void AHallwayActor::SpawnRightChildHallway() {

}

// Called when the game starts or when spawned
void AHallwayActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHallwayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
