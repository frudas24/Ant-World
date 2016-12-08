// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "AICtrl_Guard.h"
#include "Characters/Guard.h"
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

AAICtrl_Guard::AAICtrl_Guard(const class FObjectInitializer& ObjectInitializer)
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
	FoundEnemyKeyName = "FoundEnemy";
	NextWaypointKeyName = "NextWaypoint";
	IndexKeyName = "Index";
	WannaEatKeyName = "WannaEat";
	WannaPoopKeyName = "WannaPoop";
	IsLookingForFoodKeyName = "IsLookingForFood";
	FoundFoodKeyName = "FoundFood";
	ResourceKeyName = "Resource";
	UnderAttackKeyName = "UnderAttack";
	PlayerMoveToKeyName = "PlayerMoveTo";

	// Set variable default values
	CanLookForResource = true;
	UnderAttack = false;


	// Initialize PlayerState so we can assign a team index to AI
	bWantsPlayerState = true;

	// Binds event to sense other actors
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAICtrl_Guard::UpdatePerception);
}

void AAICtrl_Guard::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AGuard* Guard = Cast <AGuard>(InPawn);

	if (Guard)
	{
		Guard->OnWannaEatDelegate.AddDynamic(this, &AAICtrl_Guard::OnWannaEat);
		Guard->OnWannaPoopDelegate.AddDynamic(this, &AAICtrl_Guard::OnWannaPoop);

		if (Guard->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Guard->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*Guard->BehaviorTree);
	}

	BlackboardComp->SetValueAsBool(FoundEnemyKeyName, false);
	BlackboardComp->SetValueAsInt(IndexKeyName, 0);
}

void AAICtrl_Guard::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AAICtrl_Guard::UpdatePerception(TArray<AActor*> SensedActor)
{
	// For each SensedActor
	for (auto& TestActor : SensedActor)
	{
		if (!BlackboardComp->GetValueAsBool(IsLookingForFoodKeyName))
		{
			AEnemy* TestEnemy = Cast<AEnemy>(TestActor);

			if (TestEnemy)
			{
				AGuard* Guard = Cast <AGuard>(GetPawn());

				if (Guard)
				{
					if (!UnderAttack)
					{
						ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

						if (Controller)
						{
							Controller->Coins++;
						}
					}

					SetUnderAttack(true);

					ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(ResourceKeyName));

					if (/*!TestEnemy->IsFighting*/ !CurrentTarget && CanLookForResource && !TestEnemy->IsDead())
					{
						CanLookForResource = false;
						Guard->IsFighting = true;
						
						if (TestEnemy->SetTarget(Guard))
						{
							BlackboardComp->SetValueAsObject(ResourceKeyName, TestEnemy);
							BlackboardComp->SetValueAsVector(NextWaypointKeyName, TestEnemy->GetActorLocation());
							BlackboardComp->SetValueAsBool(FoundEnemyKeyName, true);
						}
					}
				}
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
				BlackboardComp->SetValueAsVector(NextWaypointKeyName, TestSeed->GetActorLocation());
			}
		}
	}
}

bool AAICtrl_Guard::SetTarget(ABaseCharacter* Target)
{
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(ResourceKeyName));

	if (!TestTarget || TestTarget == Target)
	{
		BlackboardComp->SetValueAsObject(ResourceKeyName, Target);
		BlackboardComp->SetValueAsVector(NextWaypointKeyName, Target->GetActorLocation());
		BlackboardComp->SetValueAsBool(FoundEnemyKeyName, true);

		return true;
	}

	else
	{
		return false;
	}
}

void AAICtrl_Guard::SetUnderAttack(bool Value)
{
	UnderAttack = Value;
	BlackboardComp->SetValueAsBool(UnderAttackKeyName, Value);
}

void AAICtrl_Guard::OnWannaEat()
{
	BlackboardComp->SetValueAsBool(WannaEatKeyName, true);
}

void AAICtrl_Guard::OnWannaPoop()
{
	BlackboardComp->SetValueAsBool(WannaPoopKeyName, true);
}

void AAICtrl_Guard::SetSightRadius(float SightRadius, float LoseSightRadius)
{
	//Sight->SightRadius = SightRadius;
	//Sight->LoseSightRadius = LoseSightRadius;
}

void AAICtrl_Guard::OnPlayerMoveTo(FVector Location)
{
	BlackboardComp->SetValueAsBool(PlayerMoveToKeyName, true);
	BlackboardComp->SetValueAsVector(NextWaypointKeyName, Location);
}