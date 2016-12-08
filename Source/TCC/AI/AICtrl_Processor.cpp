// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "AICtrl_Processor.h"
#include "Characters/Processor.h"
#include "Resources/Seed.h"
#include "Resources/ProcessedSeed.h"
#include "Characters/Enemy.h"

// Behavior Tree and Blackboard includes
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"

AAICtrl_Processor::AAICtrl_Processor(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Initialize de Perception components
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	// Sight configuration
	Sight->SightRadius = 500;
	Sight->LoseSightRadius = 600;
	Sight->PeripheralVisionAngleDegrees = 180.0f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->SetMaxAge(0.1);

	// Apply Sight to Perception Component
	PerceptionComp->ConfigureSense(*Sight);
	PerceptionComp->SetDominantSense(Sight->GetSenseImplementation());

	// Initialize the Behavior Tree and the Blackboard components
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// Fill the blackboard Name values
	LocationKeyName = "Location";
	FoundResourceKeyName = "FoundResource";
	ResourceKeyName = "Resource";
	WannaEatKeyName = "WannaEat";
	WannaPoopKeyName = "WannaPoop";
	IsActiveKeyName = "IsActive";
	IsLookingForFoodKeyName = "IsLookingForFood";
	FoundFoodKeyName = "FoundFood";
	UnderAttackKeyName = "UnderAttack";
	PlayerMoveToKeyName = "PlayerMoveTo";

	// Set variable default values
	CanLookForResource = true;
	UnderAttack = false;

	// Initialize PlayerState so we can assign a team index to AI
	bWantsPlayerState = true;

	// Binds event to sense other actors
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAICtrl_Processor::UpdatePerception);
}

void AAICtrl_Processor::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AProcessor* Processor = Cast <AProcessor>(InPawn);

	if (Processor)
	{
		Processor->OnWannaEatDelegate.AddDynamic(this, &AAICtrl_Processor::OnWannaEat);
		Processor->OnWannaPoopDelegate.AddDynamic(this, &AAICtrl_Processor::OnWannaPoop);

		if (Processor->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Processor->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*Processor->BehaviorTree);
	}
}

void AAICtrl_Processor::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AAICtrl_Processor::UpdatePerception(TArray<AActor*> SensedActor)
{
	// For each SensedActor
	for (auto& TestActor : SensedActor)
	{
		if (!UnderAttack)
		{
			if (!BlackboardComp->GetValueAsBool(IsLookingForFoodKeyName))
			{
				ASeed* TestSeed = Cast<ASeed>(TestActor);

				if (TestSeed && TestSeed->WasDelivered && CanLookForResource)
				{
					CanLookForResource = false;

					BlackboardComp->SetValueAsBool(FoundResourceKeyName, true);
					BlackboardComp->SetValueAsObject(ResourceKeyName, TestSeed);
					BlackboardComp->SetValueAsVector(LocationKeyName, TestSeed->GetActorLocation());
				}
			}

			else
			{
				AProcessedSeed* TestSeed = Cast<AProcessedSeed>(TestActor);

				if (TestSeed && !TestSeed->WasConsumed && CanLookForResource)
				{
					CanLookForResource = false;

					BlackboardComp->SetValueAsBool(FoundFoodKeyName, true);
					BlackboardComp->SetValueAsObject(ResourceKeyName, TestSeed);
					BlackboardComp->SetValueAsVector(LocationKeyName, TestSeed->GetActorLocation());
				}
			}
		}
		
		else
		{

		}

		AEnemy* TestEnemy = Cast<AEnemy>(TestActor);

		if (TestEnemy)
		{
			AProcessor* Processor = Cast <AProcessor>(GetPawn());

			if (Processor)
			{
				SetUnderAttack(true);

				ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(ResourceKeyName));

				if (/*!TestEnemy->IsFighting*/ !CurrentTarget && CanLookForResource && !TestEnemy->IsDead())
				{
					CanLookForResource = false;
					Processor->IsFighting = true;

					if (TestEnemy->SetTarget(Processor))
					{
						BlackboardComp->SetValueAsObject(ResourceKeyName, TestEnemy);
						BlackboardComp->SetValueAsVector(LocationKeyName, TestEnemy->GetActorLocation());
						BlackboardComp->SetValueAsBool(FoundResourceKeyName, true);
					}
				}
			}
		}
	}
}

bool AAICtrl_Processor::SetTarget(ABaseCharacter* Target)
{
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(ResourceKeyName));

	if (!TestTarget || TestTarget == Target)
	{
		BlackboardComp->SetValueAsObject(ResourceKeyName, Target);
		BlackboardComp->SetValueAsVector(LocationKeyName, Target->GetActorLocation());
		BlackboardComp->SetValueAsBool(FoundResourceKeyName, true);

		return true;
	}

	else
	{
		return false;
	}
}

void AAICtrl_Processor::SetUnderAttack(bool Value)
{
	UnderAttack = Value;
	BlackboardComp->SetValueAsBool(UnderAttackKeyName, Value);
}

void AAICtrl_Processor::OnWannaEat()
{
	BlackboardComp->SetValueAsBool(WannaEatKeyName, true);
}

void AAICtrl_Processor::OnWannaPoop()
{
	BlackboardComp->SetValueAsBool(WannaPoopKeyName, true);
}

void AAICtrl_Processor::SetSightRadius(float SightRadius, float LoseSightRadius)
{
	//Sight->SightRadius = SightRadius;
	//Sight->LoseSightRadius = LoseSightRadius;
}

void AAICtrl_Processor::OnPlayerMoveTo(FVector Location)
{
	BlackboardComp->SetValueAsBool(PlayerMoveToKeyName, true);
	BlackboardComp->SetValueAsVector(LocationKeyName, Location);
}