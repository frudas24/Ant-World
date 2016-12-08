// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_CollectPoop.h"
#include "AI/AICtrl_Cleaner.h"
#include "Characters/BaseCharacter.h"
#include "Nest/Nest.h"
#include "Resources/Poop.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_CollectPoop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Cleaner* MyController = Cast<AAICtrl_Cleaner>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());

		APoop* TestPoop = Cast<APoop>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->TrashKeyName));

		if (TestCharacter && TestPoop && !TestPoop->WasPicked)
		{
			TestCharacter->GetTrash(TestPoop);

			for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				ANest* Nest = *ActorItr;
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->LocationKeyName, ActorItr->DumpLocation);
			}
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}