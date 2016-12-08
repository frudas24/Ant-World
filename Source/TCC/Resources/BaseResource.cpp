// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "BaseResource.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ABaseResource::ABaseResource()
{
}

// Sets default values
ABaseResource::ABaseResource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Root component
	Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	RootComponent = Root;

	// Collider component
	Collider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Collider"));
	Collider->SetCanEverAffectNavigation(false);
	Collider->SetBoxExtent(FVector(34, 34, 88), true);
	Collider->SetRelativeLocation(FVector(0, 0, 88));
	//Collider->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	//Collider->AttachParent = RootComponent;
	Collider->SetupAttachment(RootComponent);

	// Set up a notification for when this component overlaps something
	//Collider->OnComponentBeginOverlap.AddDynamic(this, &ABaseResource::OnOverlapBegin);
	
	// Set up a notification for when this component overlaps something
	//Collider->OnComponentEndOverlap.AddDynamic(this, &ABaseResource::OnOverlapEnd);

	WasPicked = false;
	WasDelivered = false;
	WasConsumed = false;
	WasStolen = false;
}

// Called when the game starts or when spawned
void ABaseResource::BeginPlay()
{
	Super::BeginPlay();

	// Register this object can be sensed by sight
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

// Set Quantity
void ABaseResource::SetQuantity(int Quantity)
{
	this->Quantity = Quantity;
}

// Get Quantity
int ABaseResource::GetQuantity()
{
	return this->Quantity;
}