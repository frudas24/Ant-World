// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "AICtrl_Enemy.h"
#include "Characters/Enemy.h"
#include "Resources/BaseResource.h"
#include "Resources/ProcessedSeed.h"

// Behavior Tree and Blackboard includes
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"

AAICtrl_Enemy::AAICtrl_Enemy(const class FObjectInitializer& ObjectInitializer)
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
	TargetKeyName = "Target";
	FoundTargetKeyName = "FoundTarget";
	LocationKeyName = "Location";
	FlankKeyName = "Flank";
	RandomWalkKeyName = "RandomWalk";
	IndexKeyName = "Index";
	IndexAddKeyName = "IndexAdd";
	AttackKeyName = "Attack";
	//RetreatKeyName = "Retreat";

	// Set variable default values
	CanLookForTarget = true;
	IsRetreating = false;
	Index = 0;
	DamageAmount = 10;
	AttackCooldown = 1;

	// Initialize PlayerState so we can assign a team index to AI
	bWantsPlayerState = true;

	// Binds event to sense other actors
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAICtrl_Enemy::UpdatePerception);
}

void AAICtrl_Enemy::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemy* Enemy = Cast <AEnemy>(InPawn);

	if (Enemy)
	{
		if (Enemy->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Enemy->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*Enemy->BehaviorTree);
	}

	//Set default blackboar values
	BlackboardComp->SetValueAsBool(FoughtAnyKeyName, false);
	//BlackboardComp->SetValueAsBool(RetreatKeyName, false);

	if(FMath::RandBool())
	{
		BlackboardComp->SetValueAsInt(IndexAddKeyName, 4);
	}
}

void AAICtrl_Enemy::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();

	Destroy();
}

void AAICtrl_Enemy::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (IsRetreating)
	{
		OnRetreatCompletedDelegate.Broadcast(Cast<ABaseCharacter>(GetPawn()), Index);
		
		ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (Controller)
		{
			Controller->EnemiesLeft--;
		}
	}
}

void AAICtrl_Enemy::UpdatePerception(TArray<AActor*> SensedActor)
{
	// For each SensedActor
	for (auto& TestActor : SensedActor)
	{
		if(!Attack)
		{
			AProcessedSeed* TestResource = Cast<AProcessedSeed>(TestActor);

			if (TestResource && !TestResource->WasStolen && CanLookForTarget)
			{	
				CanLookForTarget = false;
				TestResource->WasStolen = true;

				BlackboardComp->SetValueAsObject(TargetKeyName, TestResource);
				BlackboardComp->SetValueAsVector(LocationKeyName, TestResource->GetActorLocation());
				BlackboardComp->SetValueAsBool(FoundTargetKeyName, true);
			}
		}

		else
		{
			ABaseCharacter* TestCharacter = Cast<ABaseCharacter>(TestActor);
			AEnemy* TestEnemy = Cast<AEnemy>(TestActor);

			if (TestCharacter && !TestEnemy)
			{
				AEnemy* Self = Cast <AEnemy>(GetPawn());

				ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(TargetKeyName));

				if (Self && (/*!TestCharacter->IsFighting*/ !CurrentTarget || CanAttackTogether) && CanLookForTarget && !TestCharacter->IsDead())
				{
					CanLookForTarget = false;
					Self->IsFighting = true;
					
					if (TestCharacter->SetTarget(Self))
					{
						Targets.AddUnique(TestCharacter);
						Self->AntsFought = Targets.Num();

						BlackboardComp->SetValueAsObject(TargetKeyName, TestCharacter);
						BlackboardComp->SetValueAsVector(LocationKeyName, TestCharacter->GetActorLocation());
						BlackboardComp->SetValueAsBool(FoundTargetKeyName, true);
					}
				}
			}
		}
	}
}

bool AAICtrl_Enemy::SetTarget(ABaseCharacter* Target)
{
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(TargetKeyName));

	if (!TestTarget || TestTarget == Target)
	{
		Targets.AddUnique(Target);
		Cast <AEnemy>(GetPawn())->AntsFought = Targets.Num();

		CanLookForTarget = false;

		BlackboardComp->SetValueAsObject(TargetKeyName, Target);
		BlackboardComp->SetValueAsVector(LocationKeyName, Target->GetActorLocation());
		BlackboardComp->SetValueAsBool(FoundTargetKeyName, true);

		return true;
	}

	else
	{
		return false;
	}
}

void AAICtrl_Enemy::SetOutputValues(bool Attack, bool AttackTogether, bool Flank, bool RandomWalk, bool InDamageAmount, bool InAttackCooldown)
{
	this->Attack = Attack;
	BlackboardComp->SetValueAsBool(AttackKeyName, Attack);

	this->CanAttackTogether = AttackTogether;

	BlackboardComp->SetValueAsBool(FlankKeyName, Flank);

	BlackboardComp->SetValueAsBool(RandomWalkKeyName, RandomWalk);

	this->DamageAmount = InDamageAmount ? 12.0f : 10.0f;

	this->AttackCooldown = InAttackCooldown ? 0.8f : 1.0f;
}

void AAICtrl_Enemy::Retreat(FVector Location, int32 Index)
{
	//BlackboardComp->SetValueAsBool(RetreatKeyName, true);
	BehaviorComp->StopTree();
	MoveTo(FAIMoveRequest(Location));
	IsRetreating = true;
	this->Index = Index;
}

void AAICtrl_Enemy::StopBehaviorTree()
{
	BehaviorComp->StopTree();
}
