// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BTService_CheckNestDigged.h"
#include "AI/AICtrl_Collector.h"
#include "Nest/Nest.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

void UBTService_CheckNestDigged::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	for (TActorIterator<ANest> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ANest* Nest = *ActorItr;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActiveKeyName, !ActorItr->CheckHasPartsToDig());
	}
}