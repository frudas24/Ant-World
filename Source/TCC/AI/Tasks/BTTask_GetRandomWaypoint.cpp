// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetRandomWaypoint.h"
#include "AI/AICtrl_Guard.h"
#include "Resources/WaypointSystem.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetRandomWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Guard* MyController = Cast<AAICtrl_Guard>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		for (TActorIterator<AWaypointSystem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AWaypointSystem* WaypointSystem = *ActorItr;

			if (WaypointSystem->Type == 0)
			{
				int32 Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(MyController->IndexKeyName);

				if (Index == 0)
				{
					Index = FMath::RandBool() ? 1 : WaypointSystem->Waypoints.Num() - 1;
				}

				else if (Index > 0 && Index < WaypointSystem->Waypoints.Num() - 1)
				{
					int32 Inc = FMath::RandRange(-1, 1);
					Index += Inc;
				}
			
				else
				{
					Index = FMath::RandBool() ? 0 : WaypointSystem->Waypoints.Num() - 2;
				}
			
				OwnerComp.GetBlackboardComponent()->SetValueAsInt(MyController->IndexKeyName, Index);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->NextWaypointKeyName, WaypointSystem->Waypoints.Last(Index));
			}
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}