// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass, TEXT("HUD"));
	if (HUD)
	{
		HUD->AddToViewport();
	}

}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromParent();
	TSubclassOf<UUserWidget> ScreenClass = bIsWinner? WinScreenClass : LoseScreenClass;
	UUserWidget* EndScreen = CreateWidget(this, ScreenClass, TEXT("EndScreen"));
	if (EndScreen)
	{
		EndScreen->AddToViewport();
	}
	
	GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
