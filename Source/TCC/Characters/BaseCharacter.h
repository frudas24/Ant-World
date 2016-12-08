// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

// Create the delegate types
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWannaEat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWannaPoop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDied, ABaseCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnderAttackChanged, bool, UnderAttack);

UCLASS()
class TCC_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// The current item on inventory. It is null when inventory is empty
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	class ABaseResource* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	int32 Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	int32 DamageGiven;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	int32 AntsFought;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	int32 SeedsStolen;

	// The hunger percentage
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	float Hunger;

	// The baddler percentage
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	float Baddler;

	// Can Rotate
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	bool Rotate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	bool IsPerformingAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	bool IsFighting;

	bool bIsDead;

	// Location to look at
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	FVector LookAtLocation;

	TSubclassOf<class APoop> PoopClass;

	// Sets default values for this character's properties
	ABaseCharacter();

	// Sets default values for this character's properties
	//ABaseCharacter(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame;
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Get Resource
	void GetResource(ABaseResource* Resource);

	// Get Landslip
	void GetLandslip(class ALandslip* Landslip);

	// Get Trash
	void GetTrash(class APoop* Poop);

	// Deliver the Resource
	void DeliverResource();

	// Take some amount of damage
	bool ApplyDamage(int32 Amount, bool Hunger);

	// Make this character die
	void Die();

	// Attack target
	bool Attack(ABaseCharacter* Target, int32 DamageAmount);

	// Check if actor is dead
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	bool IsDead();

	void AlertOtherAnts();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void Retreat();

	UFUNCTION()
	void StopAnimation();

	// Rotate towards location
	UFUNCTION()
	void RotateTowards(FVector Location);

	// Make the ant stop rotating
	UFUNCTION()
	void StopRotating();

	// Update the ants necessities
	UFUNCTION()
	void UpdateNecessities();

	bool SetTarget(ABaseCharacter* Target);

	// Declare the delegate type
	FOnWannaEat OnWannaEatDelegate;

	FOnWannaPoop OnWannaPoopDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BaseCharacterDelegate")
	FOnDied OnDiedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BaseCharacterDelegate")
	FOnUnderAttackChanged OnUnderAttackChanged;

	void Poop();

private:
	FTimerHandle RotationDelayHandler;

	FTimerHandle NecessitiesDelayHandler;

	FTimerHandle ActionHandler;
};
