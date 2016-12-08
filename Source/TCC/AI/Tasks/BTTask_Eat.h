// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Eat.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API UBTTask_Eat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboardKeyName")
	FName ResourceKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboardKeyName")
	FName FoundFoodKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboardKeyName")
	FName WannaEatKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboardKeyName")
	FName IsLookingForFoodKeyName;
};
