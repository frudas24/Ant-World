// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "Landslip.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ALandslip::ALandslip(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Root component
	Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	RootComponent = Root;

	Box = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Box"));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Box->SetBoxExtent(FVector(50), true);
	//Box->bDynamicObstacle = true;
	Box->SetCanEverAffectNavigation(true);
	Box->SetupAttachment(RootComponent);

	WasPicked = false;
}

// Called when the game starts or when spawned
void ALandslip::BeginPlay()
{
	Super::BeginPlay();

	// Register this object can be sensed by sight
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

// Called every frame
void ALandslip::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

