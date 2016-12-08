// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SeedSpawn.generated.h"

UCLASS()
class TCC_API ASeedSpawn : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", Meta = (MakeEditWidget = true))
	TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<bool> Spawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool AnySpawned;

	// Sets default values for this actor's properties
	ASeedSpawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
