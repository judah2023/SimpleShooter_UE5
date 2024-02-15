// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* EnemyShooter = Cast<AShooterCharacter>(Controller->GetPawn());
	if (EnemyShooter == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	EnemyShooter->Shoot();

	return EBTNodeResult::Succeeded;
}
