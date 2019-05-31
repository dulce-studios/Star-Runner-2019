// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

template<class C> void UHallwayJointComponent::AttachWithTransform(
	FName componentName,
	FTransform& componentTransform) {

	C* component = this->CreateDefaultSubobject<C>(componentName);
	component->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	component->SetRelativeTransform(componentTransform);
}

UHallwayJointComponent::UHallwayJointComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* staticMesh = MeshAsset.Object;
	this->SetStaticMesh(staticMesh);

	const FVector triggerBoxScale(0.075);
	const FVector arrowScale(0.025);

	FTransform triggerBoxTransform(FRotator(0), FVector(0, 0, 2), triggerBoxScale);
	FName triggerBoxName = TEXT("TriggerBox");
	this->AttachWithTransform<UBoxComponent>(triggerBoxName, triggerBoxTransform);
	this->triggerBox = Cast<UBoxComponent>(this->GetDefaultSubobjectByName(triggerBoxName));

	FTransform leftArrowTransform(FRotator(0, -90, 0), FVector(0), arrowScale);
	FName leftArrowName = TEXT("LeftArrow");
	this->AttachWithTransform<UArrowComponent>(leftArrowName, leftArrowTransform);
	this->leftArrow = Cast<UArrowComponent>(this->GetDefaultSubobjectByName(leftArrowName));
			
	FTransform rightArrowTransform(FRotator(0, 90, 0), FVector(0), arrowScale);
	FName rightArrowName = TEXT("RightArrow");
	this->AttachWithTransform<UArrowComponent>(rightArrowName, rightArrowTransform);
	this->rightArrow = Cast<UArrowComponent>(this->GetDefaultSubobjectByName(rightArrowName));
}

UArrowComponent* UHallwayJointComponent::GetLeftArrow()
{
	return this->leftArrow;
}

UArrowComponent* UHallwayJointComponent::GetRightArrow()
{
	return this->rightArrow;
}

UBoxComponent* UHallwayJointComponent::GetTriggerBox()
{
	return this->triggerBox;
}
