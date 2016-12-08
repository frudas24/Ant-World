// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_FinishDiggingPart.h"
#include "AI/AICtrl_Builder.h"
#include "Characters/BaseCharacter.h"
#include "Nest/Nest.h"
#include "Nest/NestPart.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_FinishDiggingPart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Builder* MyController = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter * TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());

		TestCharacter->IsPerformingAction = false;

		for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ANest* Nest = *ActorItr;

			// Get last part digged
			ANestPart* TestPart = Cast<ANestPart>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->NextPartToDigKeyName));

			if (TestPart)
			{
				TestPart->HidePart();
			}

			TestCharacter->IsPerformingAction = false;
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}