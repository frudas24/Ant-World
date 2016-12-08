// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "AICtrl_Builder.h"
#include "Characters/Builder.h"
#include "Resources/Landslip.h"
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

AAICtrl_Builder::AAICtrl_Builder(const class FObjectInitializer& ObjectInitializer)
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
	NextPartToDigKeyName = "NextPartToDig";
	HasPartsToDigKeyName = "HasPartsToDig";
	PartLocationKeyName = "PartLocation";
	WannaEatKeyName = "WannaEat";
	WannaPoopKeyName = "WannaPoop";
	FoundLandslipKeyName = "FoundLandslip";
	LandslipKeyName = "Landslip";
	DumpKeyName = "Dump";
	IsLookingForFoodKeyName = "IsLookingForFood";
	FoundFoodKeyName = "FoundFood";
	UnderAttackKeyName = "UnderAttack";
	PlayerMoveToKeyName = "PlayerMoveTo";

	// Set variable default values
	CanLookForLandslip = true;
	UnderAttack = false;

	// Initialize PlayerState so we can assign a team index to AI
	bWantsPlayerState = true;

	// Binds event to sense other actors
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAICtrl_Builder::UpdatePerception);
}

void AAICtrl_Builder::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ABuilder* Builder = Cast <ABuilder>(InPawn);

	if (Builder)
	{
		Builder->OnWannaEatDelegate.AddDynamic(this, &AAICtrl_Builder::OnWannaEat);
		Builder->OnWannaPoopDelegate.AddDynamic(this, &AAICtrl_Builder::OnWannaPoop);

		if (Builder->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Builder->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*Builder->BehaviorTree);
	}

	BlackboardComp->SetValueAsBool(HasPartsToDigKeyName, false);
}

void AAICtrl_Builder::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AAICtrl_Builder::UpdatePerception(TArray<AActor*> SensedActor)
{
	// For each SensedActor
	for (auto& TestActor : SensedActor)
	{
		if (!BlackboardComp->GetValueAsBool(IsLookingForFoodKeyName))
		{
			if (!UnderAttack)
			{
				ALandslip* TestLandslip = Cast<ALandslip>(TestActor);

				if (TestLandslip)
				{
					if (!TestLandslip->WasPicked  && CanLookForLandslip && !BlackboardComp->GetValueAsBool(HasPartsToDigKeyName))
					{
						CanLookForLandslip = false;

						TestLandslip->Box->SetCanEverAffectNavigation(false);

						BlackboardComp->SetValueAsObject(LandslipKeyName, TestLandslip);
						BlackboardComp->SetValueAsVector(PartLocationKeyName, TestLandslip->GetActorLocation());
						BlackboardComp->SetValueAsBool(FoundLandslipKeyName, true);
					}
				}
			}
		}

		else
		{
			AProcessedSeed* TestSeed = Cast<AProcessedSeed>(TestActor);

			if (TestSeed && !TestSeed->WasConsumed && CanLookForLandslip)
			{
				CanLookForLandslip = false;

				BlackboardComp->SetValueAsBool(FoundFoodKeyName, true);
				BlackboardComp->SetValueAsObject(LandslipKeyName, TestSeed);
				BlackboardComp->SetValueAsVector(PartLocationKeyName, TestSeed->GetActorLocation());
			}
		}

		AEnemy* TestEnemy = Cast<AEnemy>(TestActor);

		if (TestEnemy)
		{
			ABuilder* Builder = Cast <ABuilder>(GetPawn());

			if (Builder)
			{
				SetUnderAttack(true);

				ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(LandslipKeyName));

				if (/*!TestEnemy->IsFighting*/ !CurrentTarget && CanLookForLandslip && !TestEnemy->IsDead())
				{
					CanLookForLandslip = false;
					Builder->IsFighting = true;

					if (TestEnemy->SetTarget(Builder))
					{
						BlackboardComp->SetValueAsObject(LandslipKeyName, TestEnemy);
						BlackboardComp->SetValueAsVector(PartLocationKeyName, TestEnemy->GetActorLocation());
						BlackboardComp->SetValueAsBool(FoundLandslipKeyName, true);
					}
				}
			}
		}
	}
}


bool AAICtrl_Builder::SetTarget(ABaseCharacter* Target)
{
	ABaseCharacter* TestTarget = Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(LandslipKeyName));

	if(!TestTarget || TestTarget == Target)
	{
		BlackboardComp->SetValueAsObject(LandslipKeyName, Target);
		BlackboardComp->SetValueAsVector(PartLocationKeyName, Target->GetActorLocation());
		BlackboardComp->SetValueAsBool(FoundLandslipKeyName, true);

		return true;
	}

	else
	{
		return false;
	}
}

void AAICtrl_Builder::SetUnderAttack(bool Value)
{
	UnderAttack = Value;
	BlackboardComp->SetValueAsBool(UnderAttackKeyName, Value);
}

void AAICtrl_Builder::OnWannaEat()
{
	BlackboardComp->SetValueAsBool(WannaEatKeyName, true);
}

void AAICtrl_Builder::OnWannaPoop()
{
	BlackboardComp->SetValueAsBool(WannaPoopKeyName, true);
}

void AAICtrl_Builder::SetSightRadius(float SightRadius, float LoseSightRadius)
{
	//Sight->SightRadius = SightRadius;
	//Sight->LoseSightRadius = LoseSightRadius;
}

void AAICtrl_Builder::OnPlayerMoveTo(FVector Location)
{
	BlackboardComp->SetValueAsBool(PlayerMoveToKeyName, true);
	BlackboardComp->SetValueAsVector(PartLocationKeyName, Location);
}