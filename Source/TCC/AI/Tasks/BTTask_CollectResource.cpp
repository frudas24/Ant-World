// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_CollectResource.h"
#include "AI/AICtrl_Collector.h"
#include "AI/AICtrl_Enemy.h"
#include "Resources/BaseResource.h"
#include "Characters/BaseCharacter.h"
#include "Nest/Nest.h"
#include "Resources/WaypointSystem.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_CollectResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Collector* MyController = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());
	AAICtrl_Enemy* TestEnemy = Cast<AAICtrl_Enemy>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());
		
		ABaseResource* Resource = Cast<ABaseResource>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->ResourceKeyName));

		if(TestCharacter && Resource && !Resource->WasPicked)
		{
			TestCharacter->GetResource(Resource);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->ResourcesLocationKeyName, Resource->GetActorLocation());
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->KnowsWhereToGetResourcesKeyName, true);

			for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				ANest* Nest = * ActorItr;
				FVector ResourcesRoom = ActorItr->ResourcesRoom;

				float RandomX = FMath::RandRange(-150, 150);
				float RandomY = FMath::RandRange(-150, 150);
				
				ResourcesRoom.X += RandomX;
				ResourcesRoom.Y += RandomY;

				OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->ResourcesRoomKeyName, ResourcesRoom);
			}
		}

		return EBTNodeResult::Succeeded;
	}

	else if (TestEnemy)
	{
		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(TestEnemy->GetPawn());

		ABaseResource* Resource = Cast<ABaseResource>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TestEnemy->TargetKeyName));

		if (TestCharacter && Resource)
		{
			TestCharacter->GetResource(Resource);
			TestCharacter->SeedsStolen++;

			for (TActorIterator<AWaypointSystem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				AWaypointSystem* WaypointSystem = *ActorItr;

				if (WaypointSystem->Type == 1)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(TestEnemy->LocationKeyName, WaypointSystem->Waypoints.Last(0));
				}
			}
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}