// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetNestEntrance.h"
#include "Characters/BaseCharacter.h"
#include "Nest/Nest.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetNestEntrance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, FVector(900, 0, 5));
	return EBTNodeResult::Succeeded;
}