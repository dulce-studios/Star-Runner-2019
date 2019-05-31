// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayActor.h"

#include "HallwayUnitComponent.h"

#include "StarRunner2019Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include <cstdlib> // rand
#include <ctime> //srand seed
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
	std::srand(std::time(nullptr)); //seed rand

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
		playerCharacter->IsTurnable = true;
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
		auto* playerCharacter = Cast<AStarRunner2019Character>(OtherActor);
		playerCharacter->IsTurnable = false;

		AHallwayActor* childHallwayToDestroy = nullptr;
		if (playerCharacter->WentLeft) {
			childHallwayToDestroy = this->RightChildHallway;
		}
		else {
			childHallwayToDestroy = this->LeftChildHallway;
		}
		childHallwayToDestroy->LeftChildHallway->Destroy();
		childHallwayToDestroy->RightChildHallway->Destroy();
		childHallwayToDestroy->Destroy();

		this->Destroy();
	}
}

void AHallwayActor::SpawnLeftChildHallway() {
	UArrowComponent *leftArrow = this->HallwayJointComponent->GetLeftArrow();
	FTransform leftChildTransform(leftArrow->GetComponentTransform());

	leftChildTransform.AddToTranslation(FVector(200, 0, 0)); //alignment constant

	this->LeftChildHallway = this->SpawnChildActor(leftChildTransform);
}

void AHallwayActor::SpawnRightChildHallway() {
	UArrowComponent *rightArrow = this->HallwayJointComponent->GetRightArrow();
	FTransform rightChildTransform(rightArrow->GetComponentTransform());

	rightChildTransform.AddToTranslation(FVector(200, 0, 0)); //alignment constant

	this->RightChildHallway = this->SpawnChildActor(rightChildTransform);
}


AHallwayActor* AHallwayActor::SpawnChildActor(FTransform& Transform) {
	UWorld* World = this->GetWorld();
	if (!World) {
		/* This may actually crash UEditor */
		return nullptr;
	}

	FActorSpawnParameters Info;
	Info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return World->SpawnActor<AHallwayActor>(AHallwayActor::StaticClass(), Transform, Info);
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
