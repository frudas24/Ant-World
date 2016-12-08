// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetResourcesRoomLocation.h"
#include "Characters/BaseCharacter.h"
#include "Nest/Nest.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetResourcesRoomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ANest* Nest = *ActorItr;
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, ActorItr->ResourcesRoom);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}