// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	// Assigned at the Character level instead of Controller so we may use different enemy behaviors while re-using one controller.
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	// Sets default values for this actor's properties
	AEnemy();
};
