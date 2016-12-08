// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_DropResource.h"
#include "AI/AICtrl_Collector.h"
#include "AI/AICtrl_Enemy.h"
#include "Characters/Collector.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_DropResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Collector* MyController = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());
	AAICtrl_Enemy* TestEnemy = Cast<AAICtrl_Enemy>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->ResourceKeyName, nullptr);

		if (TestCharacter)
		{
			TestCharacter->DeliverResource();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->FoundResourceKeyName, false);
			MyController->CanLookForResource = true;

			ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			if (Controller)
			{
				Controller->Coins++;
			}

			return EBTNodeResult::Succeeded;
		}
	}

	else if (TestEnemy)
	{
		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(TestEnemy->GetPawn());

		if (TestCharacter)
		{
			TestCharacter->DeliverResource();

			ABaseCharacter* TestAnt = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TestEnemy->TargetKeyName));

			if(!TestAnt)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TestEnemy->FoundTargetKeyName, false);
			}

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}