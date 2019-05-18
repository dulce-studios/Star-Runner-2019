// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointActor.h"
#include "Engine/Classes/Components/BoxComponent.h"

AHallwayJointActor::AHallwayJointActor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh *StaticMesh = MeshAsset.Object;

	GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
	UBoxComponent *DeconstructorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeconstructorTriggerBox"));

	FQuat Rotation(0.0, 0.0, 0.0, 0.0);
	FVector Translation(0.0, 0.0, 1.0);
	FVector Scale(0.025, 0.025, 0.025);

	const FTransform DeconstructorTriggerBoxTransform(Rotation, Translation, Scale);

	DeconstructorTriggerBox->SetRelativeTransform(DeconstructorTriggerBoxTransform);

	DeconstructorTriggerBox->AttachToComponent(GetStaticMeshComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AHallwayJointActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHallwayJointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
