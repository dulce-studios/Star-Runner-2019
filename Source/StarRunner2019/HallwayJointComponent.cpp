// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayJointComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

UHallwayJointComponent::UHallwayJointComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayjoint.hallwayjoint'"));
	UStaticMesh* staticMesh = MeshAsset.Object;
	this->SetStaticMesh(staticMesh);

	FVector hallwayJointComponentOrigin;
	FVector hallwayJointComponentBoxExtent;
	float hallwayJointComponentSphereRadius;

	UKismetSystemLibrary::GetComponentBounds(
		this,
		hallwayJointComponentOrigin, // outargs
		hallwayJointComponentBoxExtent, // outargs
		hallwayJointComponentSphereRadius); // outargs

	float hallwayJointComponentScale = 2.0f * (hallwayJointComponentSphereRadius / hallwayJointComponentBoxExtent.X);

	FTransform hallwaySpawnManagerBoxTransform(FRotator(0), 
		hallwayJointComponentOrigin, // translation is about the origin of this Hallway Joint Component
		FVector(hallwayJointComponentScale));
	FName hallwaySpawnManagerBoxName = TEXT("HallwaySpawnManagerBox");
	this->hallwaySpawnManagerBox = AttachChildComponent(hallwaySpawnManagerBoxName, hallwaySpawnManagerBoxTransform);

	FTransform hallwayGameOverBoxTransform(FRotator(0), 
		FVector(hallwayJointComponentBoxExtent.X, 0, hallwayJointComponentBoxExtent.Z), // translation is about the farthest wrapping box face
		FVector(0.75, hallwayJointComponentScale, hallwayJointComponentScale));
	FName hallwayGameOverBoxName = TEXT("HallwayGameOverBox");
	this->hallwayGameOverBox = AttachChildComponent(hallwayGameOverBoxName, hallwayGameOverBoxTransform);
}

UBoxComponent* UHallwayJointComponent::AttachChildComponent(FName ChildComponentName, FTransform ChildComponentTransform)
{
	UBoxComponent* ChildComponent = this->CreateDefaultSubobject<UBoxComponent>(ChildComponentName);
	ChildComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ChildComponent->SetRelativeTransform(ChildComponentTransform);

	return ChildComponent;
}

UBoxComponent* UHallwayJointComponent::GetHallwaySpawnManagerBox()
{
	return this->hallwaySpawnManagerBox;
}

UBoxComponent* UHallwayJointComponent::GetHallwayGameOverBox()
{
	return this->hallwayGameOverBox;
}