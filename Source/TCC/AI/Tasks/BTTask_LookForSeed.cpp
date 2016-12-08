// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_LookForSeed.h"
#include "Resources/SeedSpawn.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_LookForSeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	for (TActorIterator<ASeedSpawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ASeedSpawn* SeedSpawn = *ActorItr;

		if (SeedSpawn)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKeyName, SeedSpawn->Locations[FMath::RandRange(0, SeedSpawn->Locations.Num() - 1)]);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

