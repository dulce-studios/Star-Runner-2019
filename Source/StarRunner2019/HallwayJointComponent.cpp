// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UHallwayJointComponent::UHallwayJointComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* staticMesh = MeshAsset.Object;
	this->SetStaticMesh(staticMesh); 
	
	FTransform triggerBoxTransform(FRotator(0), FVector(0), FVector(4));
	FName hallwaySpawnManagerBoxName = TEXT("HallwaySpawnManagerBox");
	this->hallwaySpawnManagerBox = this->CreateDefaultSubobject<UBoxComponent>(hallwaySpawnManagerBoxName);
	this->hallwaySpawnManagerBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	this->hallwaySpawnManagerBox->SetRelativeTransform(triggerBoxTransform);
}

UBoxComponent* UHallwayJointComponent::GetHallwaySpawnManagerBox()
{
	return this->hallwaySpawnManagerBox;
}
