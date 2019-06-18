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

	static ConstructorHelpers::FClassFinder<UCreditsWidget> CreditsWidget(
		TEXT("/Game/Blueprints/CreditsWidget"));

	this->StartMenu = CreateWidget<UStartMenuWidget>(
		this->GetWorld(),
		StartMenuWidget.Class);

	this->Credits = CreateWidget<UCreditsWidget>(
		this->GetWorld(),
		CreditsWidget.Class);
}

void AStartMenuHUD::StartButtonClicked()
{
	this->StartMenu->RemoveFromParent();

	FName NextLevelName = TEXT("StarRunnerMap");
	UGameplayStatics::OpenLevel(
		this->GetWorld(),
		NextLevelName);
}

void AStartMenuHUD::CreditsButtonClicked()
{
	this->StartMenu->RemoveFromParent();
	this->Credits->AddToViewport();
}

void AStartMenuHUD::ExitButtonClicked()
{
	const bool bIgnorePlatformRestrictions = false;
	UKismetSystemLibrary::QuitGame(
		this->GetWorld(),
		this->PlayerOwner,
		EQuitPreference::Quit,
		bIgnorePlatformRestrictions);
}

void AStartMenuHUD::BackButtonClicked()
{
	this->Credits->RemoveFromParent();
	this->StartMenu->AddToViewport();
}

void AStartMenuHUD::BeginPlay()
{
	UButton* StartGameButton = this->StartMenu->StartGameButton;
	StartGameButton->OnClicked.AddDynamic(
		this,
		&AStartMenuHUD::StartButtonClicked);

	UButton* CreditsButton = this->StartMenu->CreditsButton;
	CreditsButton->OnClicked.AddDynamic(
		this,
		&AStartMenuHUD::CreditsButtonClicked);

	UButton* ExitGameButton = this->StartMenu->ExitGameButton;
	ExitGameButton->OnClicked.AddDynamic(
		this,
		&AStartMenuHUD::ExitButtonClicked);

	UButton* BackButton = this->Credits->BackButton;
	BackButton->OnClicked.AddDynamic(
		this,
		&AStartMenuHUD::BackButtonClicked);

	this->StartMenu->AddToViewport();
	this->PlayerOwner->bShowMouseCursor = true;
}