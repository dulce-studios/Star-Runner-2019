// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "HallwayInterface.h"

// Add default functionality here for any IHallwayInterface functions that are not pure virtual.
FTransform* IHallwayInterface::GetTransformForComponent(USceneComponent* Component) {
	return nullptr;
}

void IHallwayInterface::AttachComponentToAnotherComponent(USceneComponent* Component, USceneComponent* OtherComponent, FTransform ComponentTransform) {
	Component->SetRelativeTransform(ComponentTransform);
	Component->AttachToComponent(OtherComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


AActor* IHallwayInterface::SpawnChildActor(AActor* ParentActor, FTransform* Transform) {
	return nullptr;
}