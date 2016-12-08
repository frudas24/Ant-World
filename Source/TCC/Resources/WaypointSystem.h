// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WaypointSystem.generated.h"

UCLASS()
class TCC_API AWaypointSystem : public AActor
{
	GENERATED_BODY()
	
public:
	// Waypoint array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint", Meta = (MakeEditWidget = true))
	TArray<FVector> Waypoints;

	// Waypoint type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	int32 Type;

	// Sets default values for this actor's properties
	AWaypointSystem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
