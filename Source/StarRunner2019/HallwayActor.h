// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Actor.h"
#include "HallwayInterface.h"
#include "HallwayJointComponent.h"

#include "HallwayActor.generated.h"

UCLASS()
class STARRUNNER2019_API AHallwayActor : public AActor, public IHallwayInterface {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHallwayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	UHallwayJointComponent* HallwayJointComponent;

	UPROPERTY(BlueprintReadOnly)
	AHallwayActor* LeftChildHallway;

	UPROPERTY(BlueprintReadOnly)
	AHallwayActor* RightChildHallway;


private:
	void Setup();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnGrandChildrenHallways();

	void DestroyChildHallways(AHallwayActor* ChildHallway);
	
	FTransform* GetTransformForComponent(USceneComponent* Component) override;
	
	AHallwayActor* SpawnChildActor(AActor* ParentActor, FTransform* Transform) override;

	FTransform AppendHallwayUnitComponents(const FVector Origin, const FVector BoxExtent, const float SphereRadius);

	const int GetRandomNumberInRange(int LowerBound, int UpperBound);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnLeftChildHallway();
	void SpawnRightChildHallway();

};
