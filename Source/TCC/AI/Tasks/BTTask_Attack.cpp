// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_Attack.h"
#include "Characters/BaseCharacter.h"

// Importing all AIControllers
#include "AIController.h"
#include "AI/AICtrl_Builder.h"
#include "AI/AICtrl_Cleaner.h"
#include "AI/AICtrl_Collector.h"
#include "AI/AICtrl_Enemy.h"
#include "AI/AICtrl_Guard.h"
#include "AI/AICtrl_Processor.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = Cast<AAIController>(OwnerComp.GetAIOwner());
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKeyName));
	ABaseCharacter* SelfActor = Cast<ABaseCharacter>(MyController->GetPawn());

	if (MyController && TestTarget)
	{

		//if(FVector::Dist(SelfActor->GetActorLocation(), TestTarget->GetActorLocation()) < 100)
		//{
			// Attack the target
			if(SelfActor->Attack(TestTarget, DamageAmount))
			{
				return EBTNodeResult::Succeeded;
			}
		//}

		/*else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, TestTarget->GetActorLocation());
			SelfActor->IsFighting = false;
			return EBTNodeResult::Failed;
		}*/
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FoundTargetKeyName, false);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKeyName, nullptr);

	AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());
	AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(OwnerComp.GetAIOwner());
	AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());
	AAICtrl_Enemy* MyControllerEnemy = Cast<AAICtrl_Enemy>(OwnerComp.GetAIOwner());
	AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(OwnerComp.GetAIOwner());
	AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(OwnerComp.GetAIOwner());

	if (MyControllerBuilder)
	{
		MyControllerBuilder->CanLookForLandslip = true;
	}
	
	else if (MyControllerCleaner)
	{
	MyControllerCleaner->CanLookForTrash = true;
	}

	else if (MyControllerCollector)
	{
		MyControllerCollector->CanLookForResource = true;
	}

	else if (MyControllerEnemy)
	{
		MyControllerEnemy->CanLookForTarget = true;
	}

	else if (MyControllerGuard)
	{
		MyControllerGuard->CanLookForResource = true;
	}

	else if (MyControllerProcessor)
	{
		MyControllerProcessor->CanLookForResource = true;
	}

	SelfActor->IsFighting = false;
	return EBTNodeResult::Failed;
}