// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayActor.h"

#include "HallwayUnitComponent.h"

#include "StarRunner2019Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include <stdlib.h> // for rand function LOL

#define HALLWAY_COUNT_MIN 1
#define HALLWAY_COUNT_MAX 5

// Sets default values
AHallwayActor::AHallwayActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent *RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	this->RootComponent = RootSceneComponent;

	this->Setup();
}

void AHallwayActor::Setup() {
	UHallwayUnitComponent *InitialHallwayUnitComponent = CreateDefaultSubobject<UHallwayUnitComponent>(TEXT("InitialHallwayUnitComponent"));
	InitialHallwayUnitComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(InitialHallwayUnitComponent, Origin, BoxExtent, SphereRadius);
	
	FTransform HallwayUnitComponentsEndTransform = this->AppendHallwayUnitComponents(Origin, BoxExtent, SphereRadius);

	this->HallwayJointComponent = CreateDefaultSubobject<UHallwayJointComponent>(TEXT("HallwayJointComponent"));
	IHallwayInterface::AttachComponentToAnotherComponent(this->HallwayJointComponent, this->GetRootComponent(), HallwayUnitComponentsEndTransform);
}


FTransform AHallwayActor::AppendHallwayUnitComponents(const FVector Origin, const FVector BoxExtent, const float SphereRadius) {
	float XLength = (BoxExtent.X * 2.0f);

	FRotator Rotator(0.0f, 0.0f, 0.0f);
	FVector Translation(XLength, 0.0f, 0.0f);
	FVector Scale(1.0f, 1.0f, 1.0f);

	int HallwayCount = this->GetRandomNumberInRange(HALLWAY_COUNT_MIN, HALLWAY_COUNT_MAX);

	for (auto i = 1; i < HallwayCount; ++i)
	{
		UHallwayUnitComponent* NextHallwayUnitComponent = CreateDefaultSubobject<UHallwayUnitComponent>(FName(*FString::Printf(TEXT("HallwayUnitComponent%d"), i)));
		FTransform HallwayUnitComponentTransform(Rotator, Translation, Scale);

		NextHallwayUnitComponent->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		NextHallwayUnitComponent->SetRelativeTransform(HallwayUnitComponentTransform);

		Translation.X += XLength;
	}

	FTransform HallwayUnitComponentsEndTransform(Rotator, Translation, Scale);
	
	return HallwayUnitComponentsEndTransform;
}

const int AHallwayActor::GetRandomNumberInRange(int LowerBound, int UpperBound) {
	return rand() % UpperBound + LowerBound;
}

void AHallwayActor::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AStarRunner2019Character::StaticClass())) {
		AStarRunner2019Character* Character = Cast<AStarRunner2019Character>(OtherActor);
		Character->IsTurnable = true;
		this->SpawnGrandChildrenHallways();
	}
}

void AHallwayActor::SpawnGrandChildrenHallways() {
	this->LeftChildHallway->SpawnLeftChildHallway();
	this->LeftChildHallway->SpawnRightChildHallway();
	this->RightChildHallway->SpawnRightChildHallway();
	this->RightChildHallway->SpawnLeftChildHallway();
}

void AHallwayActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AStarRunner2019Character::StaticClass())) {
		AStarRunner2019Character* Character = Cast<AStarRunner2019Character>(OtherActor);
		Character->IsTurnable = false;

		if (Character->WentLeft) {
			this->DestroyChildHallways(RightChildHallway);
		}
		else {
			this->DestroyChildHallways(LeftChildHallway);
		}

		this->Destroy();
	}
}

void AHallwayActor::DestroyChildHallways(AHallwayActor* ChildHallway) {
	ChildHallway->LeftChildHallway->Destroy();
	ChildHallway->RightChildHallway->Destroy();
	ChildHallway->Destroy();
}

void AHallwayActor::SpawnLeftChildHallway() {
	UArrowComponent* const LeftChildArrowComponent = Cast<UArrowComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("LeftArrowComponent")));
	FTransform* LeftChildHallwayTransform = GetTransformForComponent(LeftChildArrowComponent);
	this->LeftChildHallway = this->SpawnChildActor(this, LeftChildHallwayTransform);
}

void AHallwayActor::SpawnRightChildHallway() {
	UArrowComponent* const RightChildArrowComponent = Cast<UArrowComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("RightArrowComponent")));
	FTransform* RightChildHallwayTransform = GetTransformForComponent(RightChildArrowComponent);
	this->RightChildHallway = this->SpawnChildActor(this, RightChildHallwayTransform);
}

FTransform* AHallwayActor::GetTransformForComponent(USceneComponent* Component) {
	FTransform ArrowComponentTransform = Component->GetComponentTransform();

	FVector ArrowComponentLocation = ArrowComponentTransform.GetLocation();
	FQuat ArrowComponentRotation = ArrowComponentTransform.GetRotation();

	FVector VectorOffset(200, 0, 0);
	FVector RotationVector = UKismetMathLibrary::Quat_RotateVector(ArrowComponentRotation, VectorOffset);

	FVector ChildHallwayLocation = ArrowComponentLocation + RotationVector;
	FQuat ChildHallwayRotation = ArrowComponentRotation;
	FVector ChildHallwayScale = GetActorScale3D();

	FTransform* ChildHallwayTransform = new FTransform(ChildHallwayRotation, ChildHallwayLocation, ChildHallwayScale);

	return ChildHallwayTransform;
}

AHallwayActor* AHallwayActor::SpawnChildActor(AActor* ParentActor, FTransform* Transform) {
	UWorld* World = ParentActor->GetWorld();

	if (World) {
		FActorSpawnParameters Info;
		Info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		return World->SpawnActor<AHallwayActor>(AHallwayActor::StaticClass(), *Transform, Info);

	}
	else {
		return nullptr;
	}
}

// Called when the game starts or when spawned
void AHallwayActor::BeginPlay() {
	Super::BeginPlay();

	UBoxComponent* HallwayJointBoxComponent = Cast<UBoxComponent>(HallwayJointComponent->GetDefaultSubobjectByName(TEXT("TriggerBox")));

	HallwayJointBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHallwayActor::OnOverlapBegin);
	HallwayJointBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHallwayActor::OnOverlapEnd);
}

// Called every frame
void AHallwayActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
