// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_Poop.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_Poop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (TestCharacter)
	{
		TestCharacter->Poop();

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(WannaPoopKeyName, false);

		ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (Controller)
		{
			Controller->Coins++;
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}