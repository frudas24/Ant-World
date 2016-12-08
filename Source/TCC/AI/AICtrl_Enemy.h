// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AICtrl_Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRetreatCompleted, ABaseCharacter*, Character, int32, Index);

/**
 * 
 */
UCLASS()
class TCC_API AAICtrl_Enemy : public AAIController
{
	GENERATED_BODY()
	
	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UAIPerceptionComponent* PerceptionComp;

	class UAISenseConfig_Sight* Sight;

	AAICtrl_Enemy(const class FObjectInitializer& ObjectInitializer);

	// Called to make the AICtrl possess the Character
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

	UFUNCTION()
	void UpdatePerception(TArray<AActor*> SensedActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool CanLookForTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool CanAttackTogether;

	bool Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackCooldown;

	TArray<ABaseCharacter*> Targets;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetKeyName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName FoundTargetKeyName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName FlankKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName RandomWalkKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName FoughtAnyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IndexKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IndexAddKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName AttackKeyName;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//FName RetreatKeyName;

	bool IsRetreating;

	int32 Index;

	void Retreat(FVector Location, int32 Index);

	void StopBehaviorTree();

	UPROPERTY(BlueprintAssignable, Category = "BaseCharacterDelegate")
	FOnRetreatCompleted OnRetreatCompletedDelegate;

	bool SetTarget(class ABaseCharacter* Target);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetOutputValues(bool Attack, bool AttackTogether, bool Flank, bool RandomWalk, bool InDamageAmount, bool InAttackCooldown);
};
