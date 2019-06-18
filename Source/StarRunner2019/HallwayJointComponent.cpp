// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"
#include "Math/BoxSphereBounds.h"
#include "UObject/ConstructorHelpers.h"

UHallwayJointComponent::UHallwayJointComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* StaticMesh = MeshAsset.Object;
	this->SetStaticMesh(StaticMesh);

	const float TriggerBoxScale = 2.0f * (this->Bounds.SphereRadius / this->Bounds.BoxExtent.X);

	const FTransform SpawnTriggerBoxTransform(FRotator(0), FVector(0), FVector(TriggerBoxScale));
	this->SpawnTriggerBox = this->AttachTriggerBox(TEXT("SpawnTriggerBox"), SpawnTriggerBoxTransform);

	//that is, nest into the wall
	const FVector KillTriggerBoxTranslation(this->Bounds.BoxExtent.X, 0, this->Bounds.BoxExtent.Z);
	//flatten to allow leeway to turn
	const FVector KillTriggerBoxScale(0.75, TriggerBoxScale, TriggerBoxScale);
	FTransform KillTriggerBoxTransform(FRotator(0), 
		KillTriggerBoxTranslation,
		KillTriggerBoxScale);
	this->KillTriggerBox = this->AttachTriggerBox(TEXT("KillTriggerBox"), KillTriggerBoxTransform);
}

UBoxComponent* UHallwayJointComponent::AttachTriggerBox(FName ChildComponentName, FTransform ChildComponentTransform)
{
	UBoxComponent* ChildComponent = this->CreateDefaultSubobject<UBoxComponent>(ChildComponentName);
	ChildComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ChildComponent->SetRelativeTransform(ChildComponentTransform);

	return ChildComponent;
}

UBoxComponent* UHallwayJointComponent::GetSpawnTriggerBox()
{
	return this->SpawnTriggerBox;
}

UBoxComponent* UHallwayJointComponent::GetKillTriggerBox()
{
	return this->KillTriggerBox;
}