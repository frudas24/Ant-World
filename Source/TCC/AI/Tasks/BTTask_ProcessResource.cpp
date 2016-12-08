// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_ProcessResource.h"
#include "AI/AICtrl_Processor.h"
#include "Resources/ProcessedSeed.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_ProcessResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Processor* MyController = Cast<AAICtrl_Processor>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		ABaseResource* TestResource = Cast<ABaseResource>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->ResourceKeyName));

		if (TestResource)
		{
			TestResource->SetActorHiddenInGame(true);
			
			GetWorld()->SpawnActor<AProcessedSeed>(AProcessedSeed::StaticClass(), TestResource->GetActorLocation(), TestResource->GetActorRotation());
			
			//TSubclassOf<class AProcessedSeed> ProcessedSeedBP;
			UClass* ProcessedSeedBP = nullptr;

			static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/Nest/BP_ProcessedSeed.BP_ProcessedSeed'"));
			
			if (ItemBlueprint.Object)
			{
				ProcessedSeedBP = (UClass*)ItemBlueprint.Object->GeneratedClass;
			}

			UWorld* const World = GetWorld();
			
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Instigator = MyController->GetPawn();
				GetWorld()->SpawnActor<AProcessedSeed>(ProcessedSeedBP, TestResource->GetActorLocation(), TestResource->GetActorRotation(), SpawnParams);
			}

			OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->ResourceKeyName, nullptr);
			TestResource->Destroy();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->FoundResourceKeyName, false);
			MyController->CanLookForResource = true;

			return EBTNodeResult::Succeeded;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->ResourceKeyName, nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->FoundResourceKeyName, false);
	MyController->CanLookForResource = true;

	return EBTNodeResult::Failed;
}