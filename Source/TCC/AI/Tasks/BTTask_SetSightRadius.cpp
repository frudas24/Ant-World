// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_SetSightRadius.h"
#include "AI/AICtrl_Builder.h"
#include "AI/AICtrl_Cleaner.h"
#include "AI/AICtrl_Collector.h"
#include "AI/AICtrl_Guard.h"
#include "AI/AICtrl_Processor.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetSightRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());
	AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(OwnerComp.GetAIOwner());
	AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(OwnerComp.GetAIOwner());
	AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(OwnerComp.GetAIOwner());
	AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(OwnerComp.GetAIOwner());

	if (MyControllerBuilder)
	{
		MyControllerBuilder->SetSightRadius(SightRadius, LoseSightRadius);
		return EBTNodeResult::Succeeded;
	}

	else if (MyControllerCleaner)
	{
		MyControllerCleaner->SetSightRadius(SightRadius, LoseSightRadius);
		return EBTNodeResult::Succeeded;
	}

	else if (MyControllerCollector)
	{
		MyControllerCollector->SetSightRadius(SightRadius, LoseSightRadius);
		return EBTNodeResult::Succeeded;
	}

	else if (MyControllerGuard)
	{
		MyControllerGuard->SetSightRadius(SightRadius, LoseSightRadius);
		return EBTNodeResult::Succeeded;
	}

	else if (MyControllerProcessor)
	{
		MyControllerProcessor->SetSightRadius(SightRadius, LoseSightRadius);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}