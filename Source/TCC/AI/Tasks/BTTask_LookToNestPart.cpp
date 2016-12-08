// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_LookToNestPart.h"
#include "AI/AICtrl_Builder.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_LookToNestPart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Builder* MyController = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter * TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());

		TestCharacter->RotateTowards(OwnerComp.GetBlackboardComponent()->GetValueAsVector(MyController->PartLocationKeyName));

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}