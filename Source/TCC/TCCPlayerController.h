// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TCCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TCC_API ATCCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AntsQtt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProcessorQtt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuilderQtt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FixerQtt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CollectorQtt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GuardQtt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CleanerQtt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Period;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemiesLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Turn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProcessorDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuilderDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FixerDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CollectorDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GuardDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CleanerDead;

	ATCCPlayerController();
};
