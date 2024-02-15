// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* DeadPawn)
{
	Super::PawnKilled(DeadPawn);

	APlayerController* PlayerController = Cast<APlayerController>(DeadPawn->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	if (IsKilledAllEnemy())
	{
		EndGame(true);
	}
}

bool AKillEmAllGameMode::IsKilledAllEnemy() const
{
	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead())
		{
			return false;
		}
	}
	
	return true;
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsPlayerController = Controller->IsPlayerController();
		bool bIsWinner = (bIsPlayerController == bIsPlayerWinner);
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
