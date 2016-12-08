// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_RandomPointLocationRadius.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_RandomPointLocationRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(LocationKeyName);

	float RandomX = FMath::RandRange(-1 * Radius, Radius);
	float RandomY = FMath::RandRange(-1 * Radius, Radius);

	Location.X += RandomX;
	Location.Y += RandomY;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, Location);
	
	return EBTNodeResult::Succeeded;
}