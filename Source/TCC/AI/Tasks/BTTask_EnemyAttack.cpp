// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_EnemyAttack.h"
#include "Characters/BaseCharacter.h"

// Importing all AIControllers
#include "AI/AICtrl_Enemy.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Enemy* EnemyController = Cast<AAICtrl_Enemy>(OwnerComp.GetAIOwner());
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKeyName));
	ABaseCharacter* SelfActor = Cast<ABaseCharacter>(EnemyController->GetPawn());
	
	if (EnemyController && TestTarget)
	{	
		//if(FVector::Dist(SelfActor->GetActorLocation(), TestTarget->GetActorLocation()) < 100)
		//{
		// Attack the target
		if (SelfActor && SelfActor->Attack(TestTarget, EnemyController->DamageAmount))
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
	
	EnemyController->CanLookForTarget = true;
	
	SelfActor->IsFighting = false;
	return EBTNodeResult::Failed;
}