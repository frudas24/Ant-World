// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "TCCGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API ATCCGameMode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;

	ATCCGameMode(const FObjectInitializer& ObjectInitializer);
};
