// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "Enemy.h"

// Sets default values for this actor's properties
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Health = 100;
	DamageGiven = 0;
	AntsFought = 0;
	Hunger = FMath::RandRange(80.0f, 100.0f);
	Baddler = FMath::RandRange(0.0f, 20.0f);
	LookAtLocation = FVector(0, 0, 0);
	IsPerformingAction = false;
	Rotate = false;
	IsFighting = false;
}