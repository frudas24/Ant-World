// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_CollectLandslip.h"
#include "AI/AICtrl_Builder.h"
#include "Characters/BaseCharacter.h"
#include "Nest/Nest.h"
#include "Resources/Landslip.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_CollectLandslip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Builder* MyController = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());

		ALandslip* TestLandslip = Cast<ALandslip>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->LandslipKeyName));

		if (TestCharacter && TestLandslip && !TestLandslip->WasPicked)
		{
			TestCharacter->GetLandslip(TestLandslip);

			for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				ANest* Nest = *ActorItr;
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->DumpKeyName, ActorItr->DumpLocation);
			}

			return EBTNodeResult::Succeeded;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->LandslipKeyName, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->FoundLandslipKeyName, false);

		MyController->CanLookForLandslip = true;
	}

	return EBTNodeResult::Failed;
}