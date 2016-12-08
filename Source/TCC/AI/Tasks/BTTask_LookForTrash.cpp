// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_LookForTrash.h"
#include "Resources/WaypointSystem.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_LookForTrash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int32 Type = FMath::RandRange(0, 3);

	for (TActorIterator<AWaypointSystem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AWaypointSystem* WaypointSystem = *ActorItr;

		if (WaypointSystem->Type == Type)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, WaypointSystem->Waypoints[FMath::RandRange(0, WaypointSystem->Waypoints.Num() - 1)]);
		}
	}

	return EBTNodeResult::Succeeded;
}



