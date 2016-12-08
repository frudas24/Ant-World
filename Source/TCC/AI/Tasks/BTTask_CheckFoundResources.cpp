// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_CheckFoundResources.h"
#include "AI/AICtrl_Collector.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_CheckFoundResources::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Collector* MyController = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->KnowsWhereToGetResourcesKeyName, OwnerComp.GetBlackboardComponent()->GetValueAsBool(MyController->FoundResourceKeyName));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}