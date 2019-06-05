// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "StartMenuHUD.h"

#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"

AStartMenuHUD::AStartMenuHUD()
{
	static ConstructorHelpers::FClassFinder<UStartMenuWidget> StartMenuWidget(
		TEXT("/Game/Blueprints/StartMenuWidget"));

	this->StartMenu = CreateWidget<UStartMenuWidget>(
		this->GetWorld(),
		StartMenuWidget.Class);
}

void AStartMenuHUD::StartButtonClicked()
{
	this->StartMenu->RemoveFromParent();

	FName NextLevelName = TEXT("StarRunnerMap");
	UGameplayStatics::OpenLevel(
		this->GetWorld(),
		NextLevelName);
}

void AStartMenuHUD::BeginPlay()
{
	UButton* StartGameButton = this->StartMenu->StartGameButton;
	StartGameButton->OnClicked.AddDynamic(
		this,
		&AStartMenuHUD::StartButtonClicked);
	this->StartMenu->AddToViewport();
	this->PlayerOwner->bShowMouseCursor = true;
}