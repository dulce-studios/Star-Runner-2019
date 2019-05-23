// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"

// TODO: Refactor by using generics for the component setups

UHallwayJointComponent::UHallwayJointComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* StaticMesh = MeshAsset.Object;
	this->SetStaticMesh(StaticMesh);

	SetupDeconstructorTriggerBox();
	SetupLeftArrowComponent();
	SetupRightArrowComponent();
}

void UHallwayJointComponent::SetupDeconstructorTriggerBox() {
	UBoxComponent* DeconstructorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeconstructorTriggerBox"));

	FRotator Rotation(0.0, 0.0, 0.0);
	FVector Translation(0.0, 0.0, 2.0);
	FVector Scale(0.075, 0.075, 0.075);

	const FTransform DeconstructorTriggerBoxTransform(Rotation, Translation, Scale);

	DeconstructorTriggerBox->SetRelativeTransform(DeconstructorTriggerBoxTransform);
	DeconstructorTriggerBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	DeconstructorTriggerBox->SetVisibility(false);

}

void UHallwayJointComponent::SetupLeftArrowComponent() {
	UArrowComponent* LeftArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrowComponent"));

	FRotator Rotation(0.0, -90.0, 0.0);
	FVector Translation(0.0, 0.0, 0.0);
	FVector Scale(0.025, 0.025, 0.025);

	const FTransform LeftArrowComponentTransform(Rotation, Translation, Scale);

	LeftArrowComponent->SetRelativeTransform(LeftArrowComponentTransform);
	LeftArrowComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	LeftArrowComponent->SetVisibility(false);
}

void UHallwayJointComponent::SetupRightArrowComponent() {
	UArrowComponent* RightArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent"));

	FRotator Rotation(0.0, 90.0, 0.0);
	FVector Translation(0.0, 0.0, 0.0);
	FVector Scale(0.025, 0.025, 0.025);

	const FTransform RightArrowComponentTransform(Rotation, Translation, Scale);

	RightArrowComponent->SetRelativeTransform(RightArrowComponentTransform);
	RightArrowComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	RightArrowComponent->SetVisibility(false);
}