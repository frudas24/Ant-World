// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class TCC_API AEnemySpawn : public AActor
{
	GENERATED_BODY()

	int32 Inputs;

	int32 Outputs;

public:
	// Sets default values for this actor's properties
	AEnemySpawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//UFUNCTION(BlueprintCallable, Category = "NEAT")
	//void CreateFirstGeneration();
	
	UFUNCTION(BlueprintCallable, Category = "NEAT")
	void LoadNeatParams();

	UFUNCTION(BlueprintCallable, Category = "NEAT")
	TArray<bool> CreateFirstGeneration(bool CanSaveFile, int32 AntsQtt, int32 ProcessorQtt, int32 BuilderQtt, int32 CollectorQtt, int32 GuardQtt, int32 CleanerQtt);

	UFUNCTION(BlueprintCallable, Category = "NEAT")
	TArray<bool> CreateGenerationFromFile(bool CanSaveFile, FString FileName, int32 AntsQtt, int32 ProcessorQtt, int32 BuilderQtt, int32 CollectorQtt, int32 GuardQtt, int32 CleanerQtt);
	
	UFUNCTION(BlueprintCallable, Category = "NEAT")
	TArray<bool> Evolve(bool CanSaveFile, int32 AntsQtt, int32 ProcessorQtt, int32 BuilderQtt, int32 CollectorQtt, int32 GuardQtt, int32 CleanerQtt);

	UFUNCTION(BlueprintCallable, Category = "NEAT")
	float CalculateFitness(bool Attack, int32 DamageGiven, int32 AntsFought, int32 HealthLeft, int32 SeedsStolen, int32 AntsQtt);

	UFUNCTION(BlueprintCallable, Category = "NEAT")
	void DeletePopulation();

	UFUNCTION(BlueprintCallable, Category = "DefaultFunctions")
	class AEnemy* SpawnEnemy();

	UFUNCTION(BlueprintCallable, Category = "DefaultFunctions")
	void Retreat(bool CanSaveFile);

	UFUNCTION()
	void OnEnemyDied(ABaseCharacter* DeadCharacter);

	UFUNCTION()
	void OnEnemyRetreat(ABaseCharacter* Character, int32 Index);

	TArray<int32> DamageGiven;
	
	TArray<int32> AntsFought;
	
	TArray<int32> HealthLeft;

	TArray<int32> SeedsStolen;

	TArray<bool> Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	FVector SpawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultVariables")
	TArray<class AEnemy*> Enemies;

	TSubclassOf<class AEnemy> EnemyClass;
};
