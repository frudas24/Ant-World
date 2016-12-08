// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTTask_GetNextPartToDig.h"
#include "AI/AICtrl_Builder.h"
#include "Nest/Nest.h"
#include "Nest/NestPart.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetNextPartToDig::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICtrl_Builder* MyController = Cast<AAICtrl_Builder>(OwnerComp.GetAIOwner());

	if (MyController)
	{
		for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ANest* Nest = *ActorItr;

			// Get last part digged
			ANestPart* TestPart = Cast<ANestPart>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyController->NextPartToDigKeyName));

			int32 PartX, PartY;

			// Not the first part
			if (TestPart)
			{
				if(Nest->GetFirstPartToDig(PartX, PartY)->WasDig)
				{
					TestPart->GetCoordinates(PartX, PartY);
					TestPart = Nest->GetNextPartToDig(PartX, PartY, MyController->GetPawn());

					if (TestPart)
					{
						if (TestPart->WasDig)
						{
							ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->HasPartsToDigKeyName, false);
							TestCharacter->IsPerformingAction = false;
							
							return EBTNodeResult::Failed;
						}

						else
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->NextPartToDigKeyName, TestPart);
							OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->PartLocationKeyName, TestPart->GetActorLocation());
						}
					}

					else
					{
						ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(MyController->GetPawn());
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(MyController->HasPartsToDigKeyName, false);
						TestCharacter->IsPerformingAction = false;
						
						return EBTNodeResult::Failed;
					}
				}

				else
				{
					TestPart = Nest->GetFirstPartToDig(PartX, PartY);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->NextPartToDigKeyName, TestPart);
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->PartLocationKeyName, TestPart->GetActorLocation());
				}
			}

			// First part is always the entrance
			else
			{
				TestPart = Nest->GetFirstPartToDig(PartX, PartY);
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(MyController->NextPartToDigKeyName, TestPart);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(MyController->PartLocationKeyName, TestPart->GetActorLocation());
			}
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}