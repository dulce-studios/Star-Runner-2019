// Fill out your copyright notice in the Description page of Project Settings.

#include "HallwayUnitComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UHallwayUnitComponent::UHallwayUnitComponent()
{
    // Based on https://forums.unrealengine.com/development-discussion/c-gameplay-programming/118457-set-static-mesh-in-c
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/hallwayunit.hallwayunit'"));
    this->SetStaticMesh(MeshAsset.Object);
}