// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BaseCharacter.h"
#include "Resources/BaseResource.h"
#include "Resources/Landslip.h"
#include "Resources/Poop.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "AI/AICtrl_Builder.h"
#include "AI/AICtrl_Cleaner.h"
#include "AI/AICtrl_Collector.h"
#include "AI/AICtrl_Enemy.h"
#include "AI/AICtrl_Guard.h"
#include "AI/AICtrl_Processor.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Health = 100;
	DamageGiven = 0;
	AntsFought = 0;
	SeedsStolen = 0;
	Hunger = FMath::RandRange(80.0f, 100.0f);
	Baddler = FMath::RandRange(0.0f, 20.0f);
	LookAtLocation = FVector(0, 0, 0);
	IsPerformingAction = false;
	Rotate = false;
	IsFighting = false;
	bIsDead = false;

	static ConstructorHelpers::FObjectFinder<UBlueprint> PoopBlueprint(TEXT("Blueprint'/Game/Blueprints/Resources/BP_Poop.BP_Poop'"));

	if (PoopBlueprint.Object)
	{
		PoopClass = (UClass*)PoopBlueprint.Object->GeneratedClass;
	}
}

/*ABaseCharacter::ABaseCharacter(const FObjectInitializer & ObjectInitializer)
{
}*/

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);

	// Delay
	GetWorld()->GetTimerManager().SetTimer(NecessitiesDelayHandler, this, &ABaseCharacter::UpdateNecessities, 0.25f, false);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Rotate)
	{
		SetActorRotation(FRotator(GetActorRotation().Pitch, FMath::FInterpTo(GetActorRotation().Yaw, FRotationMatrix::MakeFromX(LookAtLocation
			- GetActorLocation()).Rotator().Yaw, GetWorld()->GetDeltaSeconds(), 15), GetActorRotation().Roll));
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

// Get the resource from the environment
void ABaseCharacter::GetResource(ABaseResource* Resource)
{
	if (Resource)
	{
		Resource->WasPicked = true;
		Inventory = Resource;
		Resource->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	}
}

// Get the Landslip from the environment
void ABaseCharacter::GetLandslip(ALandslip* Landslip)
{
	if (Landslip)
	{
		Landslip->WasPicked = true;
		Landslip->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	}
}

void ABaseCharacter::GetTrash(APoop * Poop)
{
	if (Poop)
	{
		Poop->WasPicked = true;
		Poop->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	}
}

// Deliver the resource
void ABaseCharacter::DeliverResource()
{
	if(Inventory)
	{
		Inventory->DetachRootComponentFromParent();
		Inventory->WasDelivered = true;
		Inventory = nullptr;
	}
}

// Take some amount of Damage
bool ABaseCharacter::ApplyDamage(int32 Amount, bool Hunger)
{
	if (!Hunger)
	{
		AlertOtherAnts();
	}

	Health -= Amount;

	// Died?
	if (Health <= 0)
	{
		Die();
	}

	return Health > 0;
}

void ABaseCharacter::Die()
{
	if (!bIsDead)
	{
		IsPerformingAction = false;

		AAICtrl_Enemy* MyControllerEnemy = Cast<AAICtrl_Enemy>(GetController());

		if (MyControllerEnemy)
		{
			MyControllerEnemy->StopBehaviorTree();
		}

		else
		{
			AAIController* MyController = Cast<AAIController>(GetController());

			if (MyController)
			{
				MyController->UnPossess();
				MyController->Destroy();
			}
		}

		bIsDead = true;
		OnDiedDelegate.Broadcast(this);
	}
}

void ABaseCharacter::AlertOtherAnts()
{
	for (TActorIterator<ABaseCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABaseCharacter* Ant = *ActorItr;
		
		AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(Ant->GetController());
		AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(Ant->GetController());
		AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(Ant->GetController());
		AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(Ant->GetController());
		AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(Ant->GetController());

		if (MyControllerBuilder)
		{
			MyControllerBuilder->SetUnderAttack(true);
			Ant->OnUnderAttackChanged.Broadcast(true);
		}

		else if (MyControllerCleaner)
		{
			MyControllerCleaner->SetUnderAttack(true);
			Ant->OnUnderAttackChanged.Broadcast(true);
		}

		else if (MyControllerCollector)
		{
			MyControllerCollector->SetUnderAttack(true);
			Ant->OnUnderAttackChanged.Broadcast(true);
		}

		else if (MyControllerGuard)
		{
			MyControllerGuard->SetUnderAttack(true);
			Ant->OnUnderAttackChanged.Broadcast(true);
		}

		else if (MyControllerProcessor)
		{
			MyControllerProcessor->SetUnderAttack(true);
			Ant->OnUnderAttackChanged.Broadcast(true);
		}
	}
}

void ABaseCharacter::Retreat()
{
	AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(this->GetController());
	AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(this->GetController());
	AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(this->GetController());
	AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(this->GetController());
	AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(this->GetController());

	if (MyControllerBuilder)
	{
		MyControllerBuilder->SetUnderAttack(false);
		this->OnUnderAttackChanged.Broadcast(false);
	}

	else if (MyControllerCleaner)
	{
		MyControllerCleaner->SetUnderAttack(false);
		this->OnUnderAttackChanged.Broadcast(false);
	}

	else if (MyControllerCollector)
	{
		MyControllerCollector->SetUnderAttack(false);
		this->OnUnderAttackChanged.Broadcast(false);
	}

	else if (MyControllerGuard)
	{
		MyControllerGuard->SetUnderAttack(false);
		this->OnUnderAttackChanged.Broadcast(false);
	}

	else if (MyControllerProcessor)
	{
		MyControllerProcessor->SetUnderAttack(false);
		this->OnUnderAttackChanged.Broadcast(false);
	}

	Health = 100;
}

bool ABaseCharacter::Attack(ABaseCharacter* Target, int32 DamageAmount)
{
	if(!Target->IsDead())
	{
		IsPerformingAction = true;
		bool EnemyIsAlive = Target->ApplyDamage(DamageAmount, false);
		DamageGiven += DamageAmount;
		
		// Delay to stop playing animation
		GetWorld()->GetTimerManager().SetTimer(ActionHandler, this, &ABaseCharacter::StopAnimation, 1.0f, false);

		return EnemyIsAlive;
	}

	IsFighting = false;

	return false;
}

bool ABaseCharacter::IsDead()
{
	return Health <= 0;
}

// Stops the animation
void ABaseCharacter::StopAnimation()
{
	IsPerformingAction = false;
}

// Rotate towards location
void ABaseCharacter::RotateTowards(FVector Location)
{
	LookAtLocation = Location;
	Rotate = true;
	IsPerformingAction = true;

	// Delay
	GetWorld()->GetTimerManager().SetTimer(RotationDelayHandler, this, &ABaseCharacter::StopRotating, 0.25f, false);
}

// Make the ant stop rotating
void ABaseCharacter::StopRotating()
{
	Rotate = false;
}

// Update the ants necessities
void ABaseCharacter::UpdateNecessities()
{
	Hunger -= 0.1;
	Baddler += 0.1;

	if (Hunger <= 20.0f)
	{
		OnWannaEatDelegate.Broadcast();

		if (Hunger <= 0.0f)
		{
			ApplyDamage(1, true);
		}
	}

	if (Baddler >= 90)
	{
		OnWannaPoopDelegate.Broadcast();
	}

	// Delay
	GetWorld()->GetTimerManager().SetTimer(NecessitiesDelayHandler, this, &ABaseCharacter::UpdateNecessities, 0.2f, false);
}

bool ABaseCharacter::SetTarget(ABaseCharacter* Target)
{
	AAICtrl_Builder* MyControllerBuilder = Cast<AAICtrl_Builder>(GetController());
	AAICtrl_Cleaner* MyControllerCleaner = Cast<AAICtrl_Cleaner>(GetController());
	AAICtrl_Collector* MyControllerCollector = Cast<AAICtrl_Collector>(GetController());
	AAICtrl_Enemy* MyControllerEnemy = Cast<AAICtrl_Enemy>(GetController());
	AAICtrl_Guard* MyControllerGuard = Cast<AAICtrl_Guard>(GetController());
	AAICtrl_Processor* MyControllerProcessor = Cast<AAICtrl_Processor>(GetController());

	if (MyControllerBuilder)
	{
		IsFighting = MyControllerBuilder->SetTarget(Target);
	}

	else if (MyControllerCleaner)
	{
		IsFighting = MyControllerCleaner->SetTarget(Target);
	}

	else if (MyControllerCollector)
	{
		IsFighting = MyControllerCollector->SetTarget(Target);
	}

	else if (MyControllerEnemy)
	{
		IsFighting = MyControllerEnemy->SetTarget(Target);
	}

	else if (MyControllerGuard)
	{
		IsFighting = MyControllerGuard->SetTarget(Target);
	}

	else if (MyControllerProcessor)
	{
		IsFighting = MyControllerProcessor->SetTarget(Target);
	}

	return IsFighting;
}

void ABaseCharacter::Poop()
{
	GetWorld()->SpawnActor<APoop>(PoopClass, GetActorLocation(), GetActorRotation());
	Baddler = 0;
}