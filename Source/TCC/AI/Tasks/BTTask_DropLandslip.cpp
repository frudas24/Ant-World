// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_DropLandslip.h"
#include "AI/AICtrl_Builder.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_DropLandslip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Builder* MyController = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		AActor* TestActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->LandslipKeyName));

		if (TestActor)
		{
			TestActor->DetachRootComponentFromParent();
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->LandslipKeyName, nullptr);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->FoundLandslipKeyName, false);
			
			MyController->CanLookForLandslip = true;

			ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			if (Controller)
			{
				Controller->Coins++;
			}
			
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}