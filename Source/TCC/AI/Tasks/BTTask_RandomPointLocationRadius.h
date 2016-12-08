// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomPointLocationRadius.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API UBTTask_RandomPointLocationRadius : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboarKeyName")
	FName LocationKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboarKeyName")
	float Radius;
};