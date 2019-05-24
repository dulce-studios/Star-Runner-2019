// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayActor.h"

#include "HallwayJointComponent.h"
#include "HallwayUnitComponent.h"

#include "StarRunner2019Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "Engine/Classes/Kismet/KismetSystemLibrary.h"

#include <stdlib.h> // for rand function LOL

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

	HallwayJointComponent = CreateDefaultSubobject<UHallwayJointComponent>(TEXT("HallwayJointComponent"));
	FTransform HallwayJointComponentTransform(Rotator, Translation, Scale);

	HallwayJointComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	HallwayJointComponent->SetRelativeTransform(HallwayJointComponentTransform);
}

void AHallwayActor::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AStarRunner2019Character::StaticClass())) {
		AStarRunner2019Character* Character = Cast<AStarRunner2019Character>(OtherActor);
		Character->isTurnable = true;
		SpawnGrandChildrenHallways();
	}
}

void AHallwayActor::SpawnGrandChildrenHallways() {
	LeftChildHallway->SpawnLeftChildHallway();
	LeftChildHallway->SpawnRightChildHallway();
	RightChildHallway->SpawnRightChildHallway();
	RightChildHallway->SpawnLeftChildHallway();
}

void AHallwayActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}

void AHallwayActor::SpawnLeftChildHallway() {
	UArrowComponent* const LeftChildArrowComponent = Cast<UArrowComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("LeftArrowComponent")));

	FTransform LeftChildHallwayTransform = GetTransformForComponent(LeftChildArrowComponent);
	
	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters Info;
		Info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		LeftChildHallway = World->SpawnActor<AHallwayActor>(AHallwayActor::StaticClass(), LeftChildHallwayTransform, Info);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Nooose"));
	}
}

void AHallwayActor::SpawnRightChildHallway() {
	UArrowComponent* const RightChildArrowComponent = Cast<UArrowComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("RightArrowComponent")));

	FTransform RightChildHallwayTransform = GetTransformForComponent(RightChildArrowComponent);

	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters Info;
		Info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		RightChildHallway = World->SpawnActor<AHallwayActor>(AHallwayActor::StaticClass(), RightChildHallwayTransform, Info);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Nooose"));
	}
}

FTransform AHallwayActor::GetTransformForComponent(USceneComponent* Component) {

	FTransform ArrowComponentTransform = Component->GetComponentTransform();

	FVector ArrowComponentLocation = ArrowComponentTransform.GetLocation();
	FQuat ArrowComponentRotation = ArrowComponentTransform.GetRotation();

	FVector VectorOffset(200, 0, 0);
	FVector RotationVector = UKismetMathLibrary::Quat_RotateVector(ArrowComponentRotation, VectorOffset);

	FVector ChildHallwayLocation = ArrowComponentLocation + RotationVector;
	FQuat ChildHallwayRotation = ArrowComponentRotation;
	FVector ChildHallwayScale = GetActorScale3D();

	FTransform ChildHallwayTransform(ChildHallwayRotation, ChildHallwayLocation, ChildHallwayScale);

	return ChildHallwayTransform;
}

// Called when the game starts or when spawned
void AHallwayActor::BeginPlay()
{
	Super::BeginPlay();

	UBoxComponent* HallwayJointBoxComponent = Cast<UBoxComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("TriggerBox")));

	HallwayJointBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHallwayActor::OnOverlapBegin);
}

// Called every frame
void AHallwayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
