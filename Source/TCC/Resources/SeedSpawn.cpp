// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "SeedSpawn.h"


// Sets default values
ASeedSpawn::ASeedSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASeedSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASeedSpawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

