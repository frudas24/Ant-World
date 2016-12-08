// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseResource.generated.h"

UCLASS()
class TCC_API ABaseResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Resource Quantity
	int Quantity;

	// Was the Resource already picked
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasPicked;

	// Was the Resource already delivered to the Resources Room
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasDelivered;

	// Was the Resource already consumed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasConsumed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasStolen;

	// Root Scene Component
	class USceneComponent* Root;

	// Collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* Collider;

	// Set default values
	ABaseResource();

	// Set default values
	ABaseResource(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Set Quantity
	void SetQuantity(int Quantity);

	// Get Quantity
	int GetQuantity();
};
