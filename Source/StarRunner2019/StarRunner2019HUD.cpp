// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StarRunner2019HUD.h"

#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"

AStarRunner2019HUD::AStarRunner2019HUD() 
{
	static ConstructorHelpers::FClassFinder<UStartMenuWidget> StartMenuWidget(TEXT("/Game/Blueprints/StartMenuWidget"));

	this->StartMenu = CreateWidget<UStartMenuWidget>(this->GetWorld(), StartMenuWidget.Class);
}

void AStarRunner2019HUD::StartButtonClicked() 
{
	this->StartMenu->RemoveFromParent();

	const UWorld* World = this->GetWorld();
	FName LevelName = TEXT("StarRunnerMap");

	UGameplayStatics::OpenLevel
	(
		World,
		LevelName
	);
}

void AStarRunner2019HUD::BeginPlay() 
{
	UButton* StartGameButton = this->StartMenu->StartGameButton;
	StartGameButton->OnClicked.AddDynamic(this, &AStarRunner2019HUD::StartButtonClicked);
	this->StartMenu->AddToViewport();
	this->PlayerOwner->bShowMouseCursor = true;
}