// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "WaypointSystem.h"


// Sets default values
AWaypointSystem::AWaypointSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWaypointSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypointSystem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

