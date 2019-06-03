// Fill out your copyright notice in the Description page of Project Settings.


#include "StarRunner2019HUD.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"

AStarRunner2019HUD::AStarRunner2019HUD() 
{
	static ConstructorHelpers::FClassFinder<UStartMenuWidget> StartMenuWidget(TEXT("/Game/Blueprints/StartMenuWidget"));

	this->StartMenu = CreateWidget<UStartMenuWidget>(this->GetWorld(), StartMenuWidget.Class);
}

void AStarRunner2019HUD::StartButtonClicked() 
{
	UE_LOG(LogTemp, Warning, TEXT("YEEEEEAAAAAH"));
}

void AStarRunner2019HUD::BeginPlay() 
{
	UButton* StartGameButton = this->StartMenu->StartGameButton;
	StartGameButton->OnClicked.AddDynamic(this, &AStarRunner2019HUD::StartButtonClicked);
	this->StartMenu->AddToViewport();
	this->PlayerOwner->bShowMouseCursor = true;
}