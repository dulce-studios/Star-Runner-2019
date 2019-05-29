// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"

UHallwayJointComponent::UHallwayJointComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* StaticMesh = MeshAsset.Object;
	this->SetStaticMesh(StaticMesh);

	this->SetupTriggerBox();
	this->SetupLeftArrowComponent();
	this->SetupRightArrowComponent();
}

void UHallwayJointComponent::SetupTriggerBox() {
	UBoxComponent* DeconstructorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

	FRotator Rotation(0.0, 0.0, 0.0);
	FVector Translation(0.0, 0.0, 2.0);
	FVector Scale(0.075, 0.075, 0.075);
	FTransform DeconstructorTriggerBoxTransform(Rotation, Translation, Scale);

	this->AttachComponentToSelf(DeconstructorTriggerBox, DeconstructorTriggerBoxTransform);
}

void UHallwayJointComponent::SetupLeftArrowComponent() {
	UArrowComponent* LeftArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrowComponent"));

	FRotator Rotation(0.0, -90.0, 0.0);
	FVector Translation(0.0, 0.0, 0.0);
	FVector Scale(0.025, 0.025, 0.025);
	FTransform LeftArrowComponentTransform(Rotation, Translation, Scale);

	this->AttachComponentToSelf(LeftArrowComponent, LeftArrowComponentTransform);
}

void UHallwayJointComponent::SetupRightArrowComponent() {
	UArrowComponent* RightArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent"));

	FRotator Rotation(0.0, 90.0, 0.0);
	FVector Translation(0.0, 0.0, 0.0);
	FVector Scale(0.025, 0.025, 0.025);
	FTransform RightArrowComponentTransform(Rotation, Translation, Scale);

	this->AttachComponentToSelf(RightArrowComponent, RightArrowComponentTransform);
}

void UHallwayJointComponent::AttachComponentToSelf(UPrimitiveComponent* const component, const FTransform transform) {
	component->SetRelativeTransform(transform);
	component->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	component->SetVisibility(false);
}