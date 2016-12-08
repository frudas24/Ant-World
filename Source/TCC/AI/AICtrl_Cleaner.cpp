// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "AICtrl_Cleaner.h"
#include "Characters/Cleaner.h"
#include "Resources/Poop.h"
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

AAICtrl_Cleaner::AAICtrl_Cleaner(const class FObjectInitializer& ObjectInitializer)
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
	FoundTrashKeyName = "FoundTrash";
	TrashKeyName = "Trash";
	WannaEatKeyName = "WannaEat";
	WannaPoopKeyName = "WannaPoop";
	IsActiveKeyName = "IsActive";
	IsLookingForFoodKeyName = "IsLookingForFood";
	FoundFoodKeyName = "FoundFood";
	UnderAttackKeyName = "UnderAttack";
	PlayerMoveToKeyName = "PlayerMoveTo";

	// Set variable default values
	CanLookForTrash = true;
	UnderAttack = false;

	// Initialize PlayerState so we can assign a team index to AI
	bWantsPlayerState = true;

	// Binds event to sense other actors
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAICtrl_Cleaner::UpdatePerception);
}

void AAICtrl_Cleaner::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ACleaner* Cleaner = Cast <ACleaner>(InPawn);

	if (Cleaner)
	{
		Cleaner->OnWannaEatDelegate.AddDynamic(this, &AAICtrl_Cleaner::OnWannaEat);
		Cleaner->OnWannaPoopDelegate.AddDynamic(this, &AAICtrl_Cleaner::OnWannaPoop);

		if (Cleaner->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Cleaner->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*Cleaner->BehaviorTree);
	}
}

void AAICtrl_Cleaner::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AAICtrl_Cleaner::UpdatePerception(TArray<AActor*> SensedActor)
{
	// For each SensedActor
	for (auto& TestActor : SensedActor)
	{
		if (!BlackboardComp->GetValueAsBool(IsLookingForFoodKeyName))
		{
			if (!UnderAttack)
			{
				APoop* TestPoop = Cast<APoop>(TestActor);

				if (TestPoop && !TestPoop->WasPicked && CanLookForTrash)
				{
					CanLookForTrash = false;

					BlackboardComp->SetValueAsBool(FoundTrashKeyName, true);
					BlackboardComp->SetValueAsObject(TrashKeyName, TestPoop);
					BlackboardComp->SetValueAsVector(LocationKeyName, TestPoop->GetActorLocation());
				}
			}
		}

		else
		{
			AProcessedSeed* TestSeed = Cast<AProcessedSeed>(TestActor);

			if (TestSeed && !TestSeed->WasConsumed && CanLookForTrash)
			{
				CanLookForTrash = false;

				BlackboardComp->SetValueAsBool(FoundFoodKeyName, true);
				BlackboardComp->SetValueAsObject(TrashKeyName, TestSeed);
				BlackboardComp->SetValueAsVector(LocationKeyName, TestSeed->GetActorLocation());
			}
		}

		AEnemy* TestEnemy = Cast<AEnemy>(TestActor);

		if (TestEnemy)
		{
			ACleaner* Cleaner = Cast <ACleaner>(GetPawn());

			if (Cleaner)
			{
				SetUnderAttack(true);

				ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(TrashKeyName));

				if (/*!TestEnemy->IsFighting*/ !CurrentTarget && CanLookForTrash && !TestEnemy->IsDead())
				{
					CanLookForTrash = false;
					Cleaner->IsFighting = true;
					
					if (TestEnemy->SetTarget(Cleaner))
					{
						BlackboardComp->SetValueAsObject(TrashKeyName, TestEnemy);
						BlackboardComp->SetValueAsVector(LocationKeyName, TestEnemy->GetActorLocation());
						BlackboardComp->SetValueAsBool(FoundTrashKeyName, true);
					}
				}
			}
		}
	}
}

bool AAICtrl_Cleaner::SetTarget(ABaseCharacter* Target)
{
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(TrashKeyName));

	if (!TestTarget || TestTarget == Target)
	{
		BlackboardComp->SetValueAsObject(TrashKeyName, Target);
		BlackboardComp->SetValueAsVector(LocationKeyName, Target->GetActorLocation());
		BlackboardComp->SetValueAsBool(FoundTrashKeyName, true);

		return true;
	}

	else
	{
		return false;
	}
}

void AAICtrl_Cleaner::SetUnderAttack(bool Value)
{
	UnderAttack = Value;
	BlackboardComp->SetValueAsBool(UnderAttackKeyName, Value);
}

void AAICtrl_Cleaner::OnWannaEat()
{
	BlackboardComp->SetValueAsBool(WannaEatKeyName, true);
}

void AAICtrl_Cleaner::OnWannaPoop()
{
	BlackboardComp->SetValueAsBool(WannaPoopKeyName, true);
}

void AAICtrl_Cleaner::SetSightRadius(float SightRadius, float LoseSightRadius)
{
	//Sight->SightRadius = SightRadius;
	//Sight->LoseSightRadius = LoseSightRadius;
}

void AAICtrl_Cleaner::OnPlayerMoveTo(FVector Location)
{
	BlackboardComp->SetValueAsBool(PlayerMoveToKeyName, true);
	BlackboardComp->SetValueAsVector(LocationKeyName, Location);
}