// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_Eat.h"
#include "AI/AICtrl_Builder.h"
#include "AI/AICtrl_Cleaner.h"
#include "AI/AICtrl_Collector.h"
#include "AI/AICtrl_Guard.h"
#include "AI/AICtrl_Processor.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_Eat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* TestActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ResourceKeyName));

	if (TestActor)
	{
		TestActor->Destroy();
		
		AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());
		AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(OwnerComp.GetAIOwner());
		AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());
		AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(OwnerComp.GetAIOwner());
		AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(OwnerComp.GetAIOwner());

		if (MyControllerBuilder)
		{
			MyControllerBuilder->CanLookForLandslip = true;
		}

		else if (MyControllerCleaner)
		{
			MyControllerCleaner->CanLookForTrash = true;
		}

		else if (MyControllerCollector)
		{
			MyControllerCollector->CanLookForResource = true;
		}

		else if (MyControllerGuard)
		{
			MyControllerGuard->CanLookForResource = true;
		}

		else if (MyControllerProcessor)
		{
			MyControllerProcessor->CanLookForResource = true;
		}

		ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

		if (TestCharacter)
		{
			TestCharacter->Hunger = 100;

			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ResourceKeyName, nullptr);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FoundFoodKeyName, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(WannaEatKeyName, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLookingForFoodKeyName, false);

			ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			if (Controller)
			{
				Controller->Coins++;
			}

			return EBTNodeResult::Succeeded;
		}
	}

	else
	{
		AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());
		AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(OwnerComp.GetAIOwner());
		AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());
		AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(OwnerComp.GetAIOwner());
		AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(OwnerComp.GetAIOwner());

		if (MyControllerBuilder)
		{
			MyControllerBuilder->CanLookForLandslip = true;
		}

		else if (MyControllerCleaner)
		{
			MyControllerCleaner->CanLookForTrash = true;
		}

		else if (MyControllerCollector)
		{
			MyControllerCollector->CanLookForResource = true;
		}

		else if (MyControllerGuard)
		{
			MyControllerGuard->CanLookForResource = true;
		}

		else if (MyControllerProcessor)
		{
			MyControllerProcessor->CanLookForResource = true;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ResourceKeyName, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FoundFoodKeyName, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(WannaEatKeyName, true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLookingForFoodKeyName, true);
		

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}