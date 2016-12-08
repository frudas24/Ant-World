// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Poop.generated.h"

UCLASS()
class TCC_API APoop : public AActor
{
	GENERATED_BODY()
	
public:
	// Root Scene Component
	class USceneComponent* Root;

	bool WasPicked;

	// Sets default values for this actor's properties
	APoop(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
