// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "Poop.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
APoop::APoop(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root component
	Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	RootComponent = Root;

	//WasPicked = false;
}

// Called when the game starts or when spawned
void APoop::BeginPlay()
{
	Super::BeginPlay();
	
	// Register this object can be sensed by sight
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

// Called every frame
void APoop::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

