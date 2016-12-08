// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_DropTrash.h"
#include "AI/AICtrl_Cleaner.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_DropTrash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Cleaner* MyController = Cast<AAICtrl_Cleaner>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		AActor* TestActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->TrashKeyName));

		if (TestActor)
		{
			TestActor->DetachRootComponentFromParent();
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->TrashKeyName, nullptr);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->FoundTrashKeyName, false);

			MyController->CanLookForTrash = true;

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