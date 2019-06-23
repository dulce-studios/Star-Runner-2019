// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayActor.h"

#include "HallwayUnitComponent.h"

#include "StarRunner2019Character.h"

#include "Kismet/KismetSystemLibrary.h"

#include <random>
#include <chrono>
#include <exception>
#include <sstream> //wstringstream

// Sets default values
AHallwayActor::AHallwayActor()
{
	/* Set this actor to call Tick() every frame.
	 * You can turn this off to improve performance if you don't need it.
	 */
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = this->CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	this->HallwayJointComponent = this->CreateDefaultSubobject<UHallwayJointComponent>(
		TEXT("HallwayJointComponent"));
	this->HallwayJointComponent->AttachToComponent(
		this->RootComponent,
		FAttachmentTransformRules::KeepRelativeTransform);

	float XOffset;
	{ //get hallway extents
		auto* DummyHallwayUnit =
			this->CreateDefaultSubobject<UHallwayUnitComponent>(TEXT("TempUnitHallway"));
		//as the mesh is centered, we double the length
		XOffset = DummyHallwayUnit->Bounds.BoxExtent.X * 2.0f;
		DummyHallwayUnit->DestroyComponent();
	}

	FVector HallwayUnitTranslation(0);
	const std::chrono::system_clock::rep RandomSeed =
		std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine RandomIntegerGenerator(RandomSeed);
	std::uniform_int_distribution<int> RandomDistribution(1, 5);
	const int HallwayCount = RandomDistribution(RandomIntegerGenerator);

	for (int i = 0; i < HallwayCount; i++) {
		FString NextHallwayName = FString::Printf(TEXT("HallwayUnitComponent%d"), i);
		auto* NextHallway = this->CreateDefaultSubobject<UHallwayUnitComponent>(FName(*NextHallwayName));

		NextHallway->AttachToComponent(
			this->RootComponent,
			FAttachmentTransformRules::KeepRelativeTransform);
		NextHallway->SetRelativeLocation(HallwayUnitTranslation);
		HallwayUnitTranslation += FVector(XOffset, 0, 0);
	}

	const float HallwayJointX = HallwayUnitTranslation.X - 200.0f;
	const float HallwayJointZ = HallwayUnitTranslation.Z + 10.0f;

	FVector hallwayJointTranslation(
		HallwayJointX,
		0.0f,
		HallwayJointZ);

	this->HallwayJointComponent->SetRelativeLocation(hallwayJointTranslation);
}


void AHallwayActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	const bool bOverlappedComponentIsSpawnTriggerBox =
		OverlappedComponent == this->HallwayJointComponent->GetSpawnTriggerBox();
	const bool bOtherActorIsPlayer = OtherActor->IsA(AStarRunner2019Character::StaticClass());
	if (bOverlappedComponentIsSpawnTriggerBox && bOtherActorIsPlayer) {
		auto* PlayerCharacter = Cast<AStarRunner2019Character>(OtherActor);
		PlayerCharacter->bIsTurnable = true;

		//spawn grandchildren before the player turns
		this->LeftChildHallway->SpawnLeftChildHallway();
		this->LeftChildHallway->SpawnRightChildHallway();
		this->RightChildHallway->SpawnLeftChildHallway();
		this->RightChildHallway->SpawnRightChildHallway();
	}
}

void AHallwayActor::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{

	const bool bOverlappedComponentIsSpawnTriggerBox =
		OverlappedComponent == this->HallwayJointComponent->GetSpawnTriggerBox();
	const bool bOtherActorIsPlayer = OtherActor->IsA(AStarRunner2019Character::StaticClass());
	if (bOverlappedComponentIsSpawnTriggerBox && bOtherActorIsPlayer) {
		auto* PlayerCharacter = Cast<AStarRunner2019Character>(OtherActor);
		PlayerCharacter->bIsTurnable = false;

		AHallwayActor* ChildHallwayToDestroy = nullptr;
		switch (PlayerCharacter->TurnDirection)
		{
		case EDirection::Left:
			ChildHallwayToDestroy = this->RightChildHallway;
			break;
		case EDirection::Right:
			ChildHallwayToDestroy = this->LeftChildHallway;
			break;
		default:
			//This does happen when you restart the game while colliding with the trigger box.
			//default behavior for now is to return.  May need to check for leaks
			return;
		}
		ChildHallwayToDestroy->LeftChildHallway->Destroy();
		ChildHallwayToDestroy->RightChildHallway->Destroy();
		ChildHallwayToDestroy->Destroy();

		this->Destroy();
	}
}

void AHallwayActor::SpawnLeftChildHallway()
{
	const float LeftChildHallwayYaw = -90;
	this->LeftChildHallway = this->SpawnHallway(LeftChildHallwayYaw);
}

void AHallwayActor::SpawnRightChildHallway()
{
	const float RightChildHallwayYaw = 90;
	this->RightChildHallway = this->SpawnHallway(RightChildHallwayYaw);
}

UHallwayJointComponent* AHallwayActor::GetHallwayJointComponent()
{
	return this->HallwayJointComponent;
}

AHallwayActor* AHallwayActor::SpawnHallway(float InitialRelativeYaw)
{
	UWorld* const World = this->GetWorld();
	if (!World) {
		return nullptr;
	}

	FTransform ChildTransform(this->HallwayJointComponent->GetComponentTransform());
	ChildTransform.ConcatenateRotation(FQuat(FRotator(0, InitialRelativeYaw, 0)));

	const float ZOffset = -10.0f;
	const float RotationScaleFactor = 3.33f;
	const float RotationAlignedOffset =
		this->HallwayJointComponent->Bounds.BoxExtent.X * RotationScaleFactor;

	/* Align offset by world rotation */
	const FVector TranslationOffset = ChildTransform
		.GetRotation()
		.RotateVector(FVector(RotationAlignedOffset, 0, ZOffset));
	ChildTransform.AddToTranslation(TranslationOffset);

	FActorSpawnParameters Info;
	Info.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return World->SpawnActor<AHallwayActor>(
		AHallwayActor::StaticClass(),
		ChildTransform,
		Info);
}


// Called when the game starts or when spawned
void AHallwayActor::BeginPlay() {
	Super::BeginPlay();

	UBoxComponent* SpawnTriggerBox = this->HallwayJointComponent->GetSpawnTriggerBox();
	SpawnTriggerBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&AHallwayActor::OnOverlapBegin);
	SpawnTriggerBox->OnComponentEndOverlap.AddDynamic(
		this,
		&AHallwayActor::OnOverlapEnd);

	UBoxComponent* KillTriggerBox = this->HallwayJointComponent->GetKillTriggerBox();
	KillTriggerBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&AHallwayActor::OnOverlapBegin);
	KillTriggerBox->OnComponentEndOverlap.AddDynamic(
		this,
		&AHallwayActor::OnOverlapEnd);
}

// Called every frame
void AHallwayActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
