// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckNestDigged.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API UBTService_CheckNestDigged : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackboarKeyName")
	FName IsActiveKeyName;
};