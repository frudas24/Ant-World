// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Nest.generated.h"

// Create the delegate types
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedDigging);

UCLASS()
class TCC_API ANest : public AActor
{
	GENERATED_BODY()

	#define NESTSIZE 15
	#define ROOMSIZE 4

public:
	// Sets default values for this actor's properties
	ANest();

	// Sets default values for this actor's properties
	//ANest(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Nest")
	void OpenNest();

	UFUNCTION(BlueprintCallable, Category = "Nest")
	void MazzeNest();

	UFUNCTION(BlueprintCallable, Category = "Nest")
	void RandomNest();

	// Get next part for the Builder to dig
	class ANestPart* GetNextPartToDig(int32 &PartX, int32 &PartY, APawn* Ant);

	class ANestPart* GetFirstPartToDig(int32 &PartX, int32 &PartY);

	// Matrix to build the nest
	// 5x5 food storage in one of the corners
	// 3x3 queen room
	class ANestPart* Nest[NESTSIZE * NESTSIZE];

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RoomLocations")
	FVector ResourcesRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RoomLocations")
	FVector NestRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RoomLocations")
	FVector DisposalRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RoomLocations")
	FVector QueenRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RoomLocations")
	FVector DumpLocation;

	bool CheckHasPartsToDig();

	void DigPart(int32 &PartX, int32 &PartY);

	// Declare the delegate type
	UPROPERTY(BlueprintAssignable)
	FOnFinishedDigging OnFinishedDiggingDelegate;

	bool Started;

	void SpawnLandslip();

	TSubclassOf<class ALandslip> LandslipClass;

private:
	FTimerHandle SeedSpawnHandler;
};
