// Fill out your copyright notice in the Description page of Project Settings.

#include "HallwayUnitActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AHallwayUnitActor::AHallwayUnitActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<Mesh>"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AHallwayUnitActor::BeginPlay()
{
	Super::BeginPlay();

	// Based on https://forums.unrealengine.com/development-discussion/c-gameplay-programming/1377430-oncomponenthit-isn-t-working
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AHallwayUnitActor::OnBeginOverlap);
}

// Called every frame
void AHallwayUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Based on https://answers.unrealengine.com/questions/789940/begin-overlap-in-c.html
void AHallwayUnitActor::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Help"));
	// Other Actor is the actor that triggered the event. Check that is not ourself.
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//doshit
		UE_LOG(LogTemp, Warning, TEXT("WE IN BOIS"));
	}
}
