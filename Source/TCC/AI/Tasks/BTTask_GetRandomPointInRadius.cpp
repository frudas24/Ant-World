// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetRandomPointInRadius.h"
#include "AIController.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetRandomPointInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter* TestChar = Cast<ABaseCharacter>(MyController->GetPawn());

		if (TestChar)
		{
			float RandomX = FMath::RandRange(-1500, 1500);
			float RandomY = FMath::RandRange(-1500, 1500);

			TestChar->IsFighting = false;
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, MyController->GetPawn()->GetActorLocation() + FVector(RandomX, RandomY, 0));
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}