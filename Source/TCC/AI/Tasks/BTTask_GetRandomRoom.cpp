// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetRandomRoom.h"
#include "Nest/Nest.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetRandomRoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ANest* Nest = *ActorItr;

		int Random = FMath::RandRange(0, 3);

		switch (Random)
		{
			case 0:
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, ActorItr->NestRoom);
				break;

			case 1:
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, ActorItr->QueenRoom);
				break;

			case 2:
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, ActorItr->ResourcesRoom);
				break;

			case 3:
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, ActorItr->DisposalRoom);
				break;

			default:
				break;
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}