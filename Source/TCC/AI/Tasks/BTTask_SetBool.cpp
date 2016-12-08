// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_SetBool.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_SetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(VariableKeyName, Value);

	return EBTNodeResult::Succeeded;
}