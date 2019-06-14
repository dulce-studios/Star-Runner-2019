// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UHallwayJointComponent::UHallwayJointComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* staticMesh = MeshAsset.Object;
	this->SetStaticMesh(staticMesh); 
	
	FTransform triggerBoxTransform(FRotator(0), FVector(0), FVector(4));
	FName triggerBoxName = TEXT("TriggerBox");
	this->triggerBox = this->CreateDefaultSubobject<UBoxComponent>(triggerBoxName);
	this->triggerBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	this->triggerBox->SetRelativeTransform(triggerBoxTransform);
}

UBoxComponent* UHallwayJointComponent::GetTriggerBox()
{
	return this->triggerBox;
}
