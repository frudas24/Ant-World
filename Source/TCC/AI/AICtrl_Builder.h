// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AICtrl_Builder.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API AAICtrl_Builder : public AAIController
{
	GENERATED_BODY()
	
	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UAIPerceptionComponent* PerceptionComp;

	class UAISenseConfig_Sight* Sight;

	AAICtrl_Builder(const class FObjectInitializer& ObjectInitializer);

	// Called to make the AICtrl possess the Character
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	UFUNCTION()
	void UpdatePerception(TArray<AActor*> SensedActor);

	UFUNCTION()
	void OnWannaEat();

	UFUNCTION()
	void OnWannaPoop();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool CanLookForLandslip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool UnderAttack;

	/*
	 * Builder key names
	 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName NextPartToDigKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName HasPartsToDigKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PartLocationKeyName;

	/*
	 * Fixer key names
	 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName FoundLandslipKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LandslipKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName DumpKeyName;

	/*
	 * Both key names
	 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName WannaEatKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName WannaPoopKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IsLookingForFoodKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName FoundFoodKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName UnderAttackKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerMoveToKeyName;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetUnderAttack(bool Value);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnPlayerMoveTo(FVector Location);

	bool SetTarget(class ABaseCharacter* Target);

	void SetSightRadius(float SightRadius, float LoseSightRadius);
};