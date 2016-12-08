// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetNextWaypoint.h"
#include "Resources/WaypointSystem.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	for (TActorIterator<AWaypointSystem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AWaypointSystem* WaypointSystem = *ActorItr;
		
		if (WaypointSystem->Type == 1)
		{
			int32 Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(IndexKeyName) +
				OwnerComp.GetBlackboardComponent()->GetValueAsInt(IndexAddKeyName);
			
			Index++;

			if (Index < WaypointSystem->Waypoints.Num())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsInt(IndexKeyName, OwnerComp.GetBlackboardComponent()->GetValueAsInt(IndexKeyName) + 1);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, WaypointSystem->Waypoints.Last(Index));
			}
		}
	}

	return EBTNodeResult::Succeeded;
}