// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayActor.h"

#include "HallwayUnitComponent.h"

#include "StarRunner2019Character.h"

#include "Kismet/KismetSystemLibrary.h"

#include <cstdlib> // rand
#include <ctime> //srand seed
#include <exception>
#include <sstream> //wstringstream

// Sets default values
AHallwayActor::AHallwayActor() {
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

	FTransform hallwayTransform(FRotator(0), FVector(0), FVector(1));

	float xOffset;
	{ //get hallway extents
		auto* hallwayComponent =
			this->CreateDefaultSubobject<UHallwayUnitComponent>(TEXT("TempUnitHallway"));
		FVector hallwayOrigin;
		FVector hallwayBoxExtent;
		float hallwaySphereRadius;
		UKismetSystemLibrary::GetComponentBounds(
			hallwayComponent,
			hallwayOrigin, //outarg
			hallwayBoxExtent, //outarg
			hallwaySphereRadius); //outarg
		//as the mesh is centered, we double the length
		xOffset = hallwayBoxExtent.X * 2.0f; 
		hallwayComponent->DestroyComponent();
	}

	std::srand(std::time(nullptr)); //seed rand
	const int numHallways = 1 + (std::rand() % 5); //[1...5]
	for (int i = 0; i < numHallways; i++)
	{
		std::wstringstream hallwayName(L"HallwayUnitComponent");
		hallwayName << i;
		auto* nextHallway = this->CreateDefaultSubobject<UHallwayUnitComponent>(
			FName(hallwayName.str().c_str()));

		nextHallway->AttachToComponent(
			this->RootComponent,
			FAttachmentTransformRules::KeepRelativeTransform);
		nextHallway->SetRelativeTransform(hallwayTransform);
		hallwayTransform.AddToTranslation(FVector(xOffset, 0, 0));
	}
	this->HallwayJointComponent->SetRelativeTransform(hallwayTransform);
}

void AHallwayActor::OnOverlapBegin(
	UPrimitiveComponent* OverlapComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) {

	if (OtherActor->IsA(AStarRunner2019Character::StaticClass())) {
		auto* playerCharacter = Cast<AStarRunner2019Character>(OtherActor);
		playerCharacter->bIsTurnable = true;
		//spawn grandchildren before the player turns
		this->LeftChildHallway->SpawnLeftChildHallway();
		this->LeftChildHallway->SpawnRightChildHallway();
		this->RightChildHallway->SpawnLeftChildHallway();
		this->RightChildHallway->SpawnRightChildHallway();
	}
}

void AHallwayActor::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex) {

	if (OtherActor->IsA(AStarRunner2019Character::StaticClass())) {
		auto* PlayerCharacter = Cast<AStarRunner2019Character>(OtherActor);
		PlayerCharacter->bIsTurnable = false;

		AHallwayActor* ChildHallwayToDestroy = nullptr;
		switch (PlayerCharacter->TurnDirection) {
		case EDirection::Left:
			ChildHallwayToDestroy = this->RightChildHallway;
			break;
		case EDirection::Right:
			ChildHallwayToDestroy = this->LeftChildHallway;
			break;
		default:
			throw new std::logic_error("Invalid Turn Direction");
		}
		ChildHallwayToDestroy->LeftChildHallway->Destroy();
		ChildHallwayToDestroy->RightChildHallway->Destroy();
		ChildHallwayToDestroy->Destroy();

		this->Destroy();
	}
}

void AHallwayActor::SpawnLeftChildHallway() {
	this->LeftChildHallway = this->SpawnHallFromYawAndOffset(-90, 200);
}

void AHallwayActor::SpawnRightChildHallway() {
	this->RightChildHallway = this->SpawnHallFromYawAndOffset(90, 200);
}

AHallwayActor* AHallwayActor::SpawnHallFromYawAndOffset(
	float yawDegrees,
	float rotationAlignedOffset) {

	UWorld* World = this->GetWorld();
	if (!World) {
		/* This may actually crash UEditor */
		return nullptr;
	}

	FTransform childTransform(this->HallwayJointComponent->GetComponentTransform());
	childTransform.ConcatenateRotation(FQuat(FRotator(0, yawDegrees, 0)));
	/* Align offset by world rotation */
	FVector offset = childTransform
		.GetRotation()
		.RotateVector(FVector(rotationAlignedOffset, 0, 0));
	childTransform.AddToTranslation(offset);

	FActorSpawnParameters Info;
	Info.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return World->SpawnActor<AHallwayActor>(
		AHallwayActor::StaticClass(),
		childTransform,
		Info);
}


// Called when the game starts or when spawned
void AHallwayActor::BeginPlay() {
	Super::BeginPlay();

	UBoxComponent* triggerBox = this->HallwayJointComponent->GetTriggerBox();

	triggerBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&AHallwayActor::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(
		this,
		&AHallwayActor::OnOverlapEnd);
}

// Called every frame
void AHallwayActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
