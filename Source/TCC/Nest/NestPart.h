// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NestPart.generated.h"

UCLASS()
class TCC_API ANestPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANestPart();

	ANestPart(const class FObjectInitializer& ObjectInitializer);

	ANestPart(int32 NewI, int32 NewJ, bool NewValue);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY()
	int32 i;

	UPROPERTY()
	int32 j;

	UPROPERTY()
	int32 x;

	UPROPERTY()
	int32 y;

	UPROPERTY()
	bool Value;

	UPROPERTY()
	bool WasDig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

	bool GetValue(int32 NewI, int32 NewJ);

	void SetLocation(int32 NewX, int32 NewY);

	void SetValue(int32 NewI, int32 NewJ, bool NewValue);

	void GetCoordinates(int32 &NewI, int32 &NewJ);

	void HidePart();
};
