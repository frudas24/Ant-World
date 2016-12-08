// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "Nest.h"
#include "NestPart.h"
#include "Resources/Landslip.h"

// Sets default values
ANest::ANest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DumpLocation = FVector(1900, -1900, 0);
	Started = false;

	static ConstructorHelpers::FObjectFinder<UBlueprint> LandslipBlueprint(TEXT("Blueprint'/Game/Blueprints/Resources/BP_Landslip.BP_Landslip'"));

	if (LandslipBlueprint.Object)
	{
		LandslipClass = (UClass*)LandslipBlueprint.Object->GeneratedClass;
	}
}

/*ANest::ANest(const FObjectInitializer & ObjectInitializer)
{
}*/

// Called when the game starts or when spawned
void ANest::BeginPlay()
{
	Super::BeginPlay();

	DumpLocation = FVector(1900, -1900, 0);

	// Spawn parts
	for (int i = 0, xCount = -700; i < NESTSIZE; i++, xCount = xCount + 100)
	{
		for (int j = 0, yCount = -700; j < NESTSIZE; j++, yCount = yCount + 100)
		{
			Nest[(i * NESTSIZE) + j] = GetWorld()->SpawnActor<ANestPart>(ANestPart::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
			Nest[(i * NESTSIZE) + j]->SetValue(i, j, true);
			Nest[(i * NESTSIZE) + j]->SetLocation(xCount, yCount);
		}
	}
}

void ANest::OpenNest()
{
	float x = 0, y = 0;

	for (int i = 2; i < NESTSIZE - 2; i++)
	{
		for (int j = 2; j < NESTSIZE - 2; j++)
		{
			Nest[(i * NESTSIZE) + j]->SetValue(i, j, false);
			x += Nest[(i * NESTSIZE) + j]->GetActorLocation().X;
			y += Nest[(i * NESTSIZE) + j]->GetActorLocation().Y;
			Nest[(i * NESTSIZE) + j]->HidePart();
		}
	}

	// Left side of entrance
	Nest[(13 * NESTSIZE) + 3]->SetValue(13, 3, false);
	Nest[(13 * NESTSIZE) + 3]->HidePart();
	Nest[(13 * NESTSIZE) + 4]->SetValue(13, 4, false);
	Nest[(13 * NESTSIZE) + 4]->HidePart();

	// Right side of entrance
	Nest[(13 * NESTSIZE) + 8]->SetValue(13, 8, false);
	Nest[(13 * NESTSIZE) + 8]->HidePart();
	Nest[(13 * NESTSIZE) + 9]->SetValue(13, 9, false);
	Nest[(13 * NESTSIZE) + 9]->HidePart();
	
	// Right Wall
	Nest[(11 * NESTSIZE) + 13]->SetValue(11, 13, false);
	Nest[(11 * NESTSIZE) + 13]->HidePart();
	Nest[(10 * NESTSIZE) + 13]->SetValue(10, 13, false);
	Nest[(10 * NESTSIZE) + 13]->HidePart();
	Nest[(9 * NESTSIZE) + 13]->SetValue(9, 13, false);
	Nest[(9 * NESTSIZE) + 13]->HidePart();
	Nest[(8 * NESTSIZE) + 13]->SetValue(8, 13, false);
	Nest[(8 * NESTSIZE) + 13]->HidePart();
	Nest[(4 * NESTSIZE) + 13]->SetValue(4, 13, false);
	Nest[(4 * NESTSIZE) + 13]->HidePart();
	Nest[(3 * NESTSIZE) + 13]->SetValue(3, 13, false);
	Nest[(3 * NESTSIZE) + 13]->HidePart();

	// Left Wall
	Nest[(11 * NESTSIZE) + 1]->SetValue(11, 1, false);
	Nest[(11 * NESTSIZE) + 1]->HidePart();
	Nest[(10 * NESTSIZE) + 1]->SetValue(10, 1, false);
	Nest[(10 * NESTSIZE) + 1]->HidePart();
	Nest[(8 * NESTSIZE) + 1]->SetValue(8, 1, false);
	Nest[(8 * NESTSIZE) + 1]->HidePart();
	Nest[(7 * NESTSIZE) + 1]->SetValue(7, 1, false);
	Nest[(7 * NESTSIZE) + 1]->HidePart();
	Nest[(6 * NESTSIZE) + 1]->SetValue(6, 1, false);
	Nest[(6 * NESTSIZE) + 1]->HidePart();
	Nest[(3 * NESTSIZE) + 1]->SetValue(3, 1, false);
	Nest[(3 * NESTSIZE) + 1]->HidePart();
	Nest[(2 * NESTSIZE) + 1]->SetValue(2, 1, false);
	Nest[(2 * NESTSIZE) + 1]->HidePart();
	Nest[(1 * NESTSIZE) + 1]->SetValue(1, 1, false);
	Nest[(1 * NESTSIZE) + 1]->HidePart();

	// Bottom Wall
	Nest[(1 * NESTSIZE) + 11]->SetValue(1, 11, false);
	Nest[(1 * NESTSIZE) + 11]->HidePart();
	Nest[(1 * NESTSIZE) + 10]->SetValue(1, 10, false);
	Nest[(1 * NESTSIZE) + 10]->HidePart();
	Nest[(1 * NESTSIZE) + 9]->SetValue(1, 9, false);
	Nest[(1 * NESTSIZE) + 9]->HidePart();
	Nest[(1 * NESTSIZE) + 8]->SetValue(1, 8, false);
	Nest[(1 * NESTSIZE) + 8]->HidePart();
	Nest[(1 * NESTSIZE) + 4]->SetValue(1, 4, false);
	Nest[(1 * NESTSIZE) + 4]->HidePart();
	Nest[(1 * NESTSIZE) + 3]->SetValue(1, 3, false);
	Nest[(1 * NESTSIZE) + 3]->HidePart();
	Nest[(1 * NESTSIZE) + 2]->SetValue(1, 2, false);
	Nest[(1 * NESTSIZE) + 2]->HidePart();

	// Entrance
	Nest[(13 * NESTSIZE) + 7]->SetValue(13, 7, false);
	Nest[(13 * NESTSIZE) + 7]->HidePart();
	Nest[(14 * NESTSIZE) + 7]->SetValue(14, 7, false);
	Nest[(14 * NESTSIZE) + 7]->HidePart();

	ResourcesRoom = FVector(-350, 350, 50);
	NestRoom = FVector(350, 350, 50);
	DisposalRoom = FVector(350, -350, 50);
	QueenRoom = FVector(-350, -350, 50);

	// Delay
	GetWorld()->GetTimerManager().SetTimer(SeedSpawnHandler, this, &ANest::SpawnLandslip, FMath::RandRange(10.0f, 30.0f), false);
}

void ANest::RandomNest()
{
	// First Room - Nest
	int row1 = FMath::RandRange(0, 1) * 2 + 1;
	int column1 = FMath::RandRange(0, 1) * 2 + 1;
	float x = 0, y = 0;

	for (int i = row1; i < row1 + ROOMSIZE; i++)
	{
		for (int j = column1; j < column1 + ROOMSIZE; j++)
		{
			Nest[(i * NESTSIZE) + j]->SetValue(i, j, false);
			x += Nest[(i * NESTSIZE) + j]->GetActorLocation().X;
			y += Nest[(i * NESTSIZE) + j]->GetActorLocation().Y;
			Nest[(i * NESTSIZE) + j]->HidePart();
		}
	}

	NestRoom.X = x / (ROOMSIZE * ROOMSIZE);
	NestRoom.Y = y / (ROOMSIZE * ROOMSIZE);
	NestRoom.Z = 50;

	// Second Room - Resource
	int row2 = FMath::RandRange(0, 1) * 2 + 1;
	int column2 = FMath::RandRange(0, 1) * 2 + 8;
	x = y = 0;

	for (int i = row2; i < row2 + ROOMSIZE; i++)
	{
		for (int j = column2; j < column2 + ROOMSIZE; j++)
		{
			Nest[(i * NESTSIZE) + j]->SetValue(i, j, false);
			x += Nest[(i * NESTSIZE) + j]->GetActorLocation().X;
			y += Nest[(i * NESTSIZE) + j]->GetActorLocation().Y;
			Nest[(i * NESTSIZE) + j]->HidePart();
		}
	}

	ResourcesRoom.X = x / (ROOMSIZE * ROOMSIZE);
	ResourcesRoom.Y = y / (ROOMSIZE * ROOMSIZE);
	ResourcesRoom.Z = 50;

	// Third Room
	int row3 = FMath::RandRange(0, 1) * 2 + 8;
	int column3 = FMath::RandRange(0, 1) * 2 + 1;
	x = y = 0;

	for (int i = row3; i < row3 + ROOMSIZE; i++)
	{
		for (int j = column3; j < column3 + ROOMSIZE; j++)
		{
			Nest[(i * NESTSIZE) + j]->SetValue(i, j, false);
			x += Nest[(i * NESTSIZE) + j]->GetActorLocation().X;
			y += Nest[(i * NESTSIZE) + j]->GetActorLocation().Y;
			Nest[(i * NESTSIZE) + j]->HidePart();
		}
	}

	DisposalRoom.X = x / (ROOMSIZE * ROOMSIZE);
	DisposalRoom.Y = y / (ROOMSIZE * ROOMSIZE);
	DisposalRoom.Z = 50;

	// Fourth Room
	int row4 = FMath::RandRange(0, 1) * 2 + 8;
	int column4 = FMath::RandRange(0, 1) * 2 + 8;
	x = y = 0;
	
	for (int i = row4; i < row4 + ROOMSIZE; i++)
	{
		for (int j = column4; j < column4 + ROOMSIZE; j++)
		{
			Nest[(i * NESTSIZE) + j]->SetValue(i, j, false);
			x += Nest[(i * NESTSIZE) + j]->GetActorLocation().X;
			y += Nest[(i * NESTSIZE) + j]->GetActorLocation().Y;
			Nest[(i * NESTSIZE) + j]->HidePart();
		}
	}

	QueenRoom.X = x / (ROOMSIZE * ROOMSIZE);
	QueenRoom.Y = y / (ROOMSIZE * ROOMSIZE);
	QueenRoom.Z = 50;

	// Connections
	Nest[(5 * NESTSIZE) + 3]->SetValue(5, 3, false);
	Nest[(5 * NESTSIZE) + 3]->HidePart();
	Nest[(6 * NESTSIZE) + 3]->SetValue(6, 3, false);
	Nest[(6 * NESTSIZE) + 3]->HidePart();
	Nest[(7 * NESTSIZE) + 3]->SetValue(7, 3, false); 
	Nest[(7 * NESTSIZE) + 3]->HidePart();
	Nest[(8 * NESTSIZE) + 3]->SetValue(8, 3, false); 
	Nest[(8 * NESTSIZE) + 3]->HidePart();
	Nest[(9 * NESTSIZE) + 3]->SetValue(9, 3, false);
	Nest[(9 * NESTSIZE) + 3]->HidePart();
	Nest[(3 * NESTSIZE) + 5]->SetValue(3, 5, false);
	Nest[(3 * NESTSIZE) + 5]->HidePart();
	Nest[(3 * NESTSIZE) + 6]->SetValue(3, 6, false); 
	Nest[(3 * NESTSIZE) + 6]->HidePart();
	Nest[(3 * NESTSIZE) + 7]->SetValue(3, 7, false); 
	Nest[(3 * NESTSIZE) + 7]->HidePart();
	Nest[(3 * NESTSIZE) + 8]->SetValue(3, 8, false);
	Nest[(3 * NESTSIZE) + 8]->HidePart();
	Nest[(3 * NESTSIZE) + 9]->SetValue(3, 9, false);
	Nest[(3 * NESTSIZE) + 9]->HidePart();
	Nest[(5 * NESTSIZE) + 11]->SetValue(5, 11, false);
	Nest[(5 * NESTSIZE) + 11]->HidePart();
	Nest[(6 * NESTSIZE) + 11]->SetValue(6, 11, false);
	Nest[(6 * NESTSIZE) + 11]->HidePart();
	Nest[(7 * NESTSIZE) + 11]->SetValue(7, 11, false);
	Nest[(7 * NESTSIZE) + 11]->HidePart();
	Nest[(8 * NESTSIZE) + 11]->SetValue(8, 11, false);
	Nest[(8 * NESTSIZE) + 11]->HidePart();
	Nest[(9 * NESTSIZE) + 11]->SetValue(9, 11, false);
	Nest[(9 * NESTSIZE) + 11]->HidePart();
	Nest[(11 * NESTSIZE) + 5]->SetValue(11, 5, false);
	Nest[(11 * NESTSIZE) + 5]->HidePart();
	Nest[(11 * NESTSIZE) + 6]->SetValue(11, 6, false);
	Nest[(11 * NESTSIZE) + 6]->HidePart();
	Nest[(11 * NESTSIZE) + 7]->SetValue(11, 7, false);
	Nest[(11 * NESTSIZE) + 7]->HidePart();
	Nest[(11 * NESTSIZE) + 8]->SetValue(11, 8, false);
	Nest[(11 * NESTSIZE) + 8]->HidePart();
	Nest[(11 * NESTSIZE) + 9]->SetValue(11, 9, false);
	Nest[(11 * NESTSIZE) + 9]->HidePart();

	// Entrance
	Nest[(12 * NESTSIZE) + 7]->SetValue(12, 7, false);
	Nest[(12 * NESTSIZE) + 7]->HidePart();
	Nest[(13 * NESTSIZE) + 7]->SetValue(13, 7, false);
	Nest[(13 * NESTSIZE) + 7]->HidePart();
	Nest[(14 * NESTSIZE) + 7]->SetValue(14, 7, false);
	Nest[(14 * NESTSIZE) + 7]->HidePart();

	// Delay
	GetWorld()->GetTimerManager().SetTimer(SeedSpawnHandler, this, &ANest::SpawnLandslip, FMath::RandRange(10.0f, 30.0f), false);
}

void ANest::MazzeNest()
{
	Nest[(14 * NESTSIZE) + 6]->SetValue(14, 6, false);
	Nest[(14 * NESTSIZE) + 6]->HidePart();

	Nest[(13 * NESTSIZE) + 11]->SetValue(13, 11, false);
	Nest[(13 * NESTSIZE) + 11]->HidePart();
	Nest[(13 * NESTSIZE) + 10]->SetValue(13, 10, false);
	Nest[(13 * NESTSIZE) + 10]->HidePart();
	Nest[(13 * NESTSIZE) + 9]->SetValue(13, 9, false);
	Nest[(13 * NESTSIZE) + 9]->HidePart();
	Nest[(13 * NESTSIZE) + 8]->SetValue(13, 8, false);
	Nest[(13 * NESTSIZE) + 8]->HidePart();
	Nest[(13 * NESTSIZE) + 7]->SetValue(13, 7, false);
	Nest[(13 * NESTSIZE) + 7]->HidePart();
	Nest[(13 * NESTSIZE) + 6]->SetValue(13, 6, false);
	Nest[(13 * NESTSIZE) + 6]->HidePart();

	Nest[(13 * NESTSIZE) + 1]->SetValue(13, 1, false);
	Nest[(13 * NESTSIZE) + 1]->HidePart();

	Nest[(12 * NESTSIZE) + 13]->SetValue(12, 13, false);
	Nest[(12 * NESTSIZE) + 13]->HidePart();

	Nest[(12 * NESTSIZE) + 11]->SetValue(12, 11, false);
	Nest[(12 * NESTSIZE) + 11]->HidePart();

	Nest[(12 * NESTSIZE) + 6]->SetValue(12, 6, false);
	Nest[(12 * NESTSIZE) + 6]->HidePart();
	Nest[(12 * NESTSIZE) + 5]->SetValue(12, 5, false);
	Nest[(12 * NESTSIZE) + 5]->HidePart();
	Nest[(12 * NESTSIZE) + 4]->SetValue(12, 4, false);
	Nest[(12 * NESTSIZE) + 4]->HidePart();
	Nest[(12 * NESTSIZE) + 3]->SetValue(12, 3, false);
	Nest[(12 * NESTSIZE) + 3]->HidePart();
	Nest[(12 * NESTSIZE) + 2]->SetValue(12, 2, false);
	Nest[(12 * NESTSIZE) + 2]->HidePart();
	Nest[(12 * NESTSIZE) + 1]->SetValue(12, 1, false);
	Nest[(12 * NESTSIZE) + 1]->HidePart();

	Nest[(11 * NESTSIZE) + 13]->SetValue(11, 13, false);
	Nest[(11 * NESTSIZE) + 13]->HidePart();

	Nest[(11 * NESTSIZE) + 11]->SetValue(11, 11, false);
	Nest[(11 * NESTSIZE) + 11]->HidePart();

	Nest[(11 * NESTSIZE) + 7]->SetValue(11, 7, false);
	Nest[(11 * NESTSIZE) + 7]->HidePart();

	Nest[(11 * NESTSIZE) + 5]->SetValue(11, 5, false);
	Nest[(11 * NESTSIZE) + 5]->HidePart();

	Nest[(11 * NESTSIZE) + 1]->SetValue(11, 1, false);
	Nest[(11 * NESTSIZE) + 1]->HidePart();

	Nest[(10 * NESTSIZE) + 13]->SetValue(10, 13, false);
	Nest[(10 * NESTSIZE) + 13]->HidePart();

	Nest[(10 * NESTSIZE) + 11]->SetValue(10, 11, false);
	Nest[(10 * NESTSIZE) + 11]->HidePart();
	Nest[(10 * NESTSIZE) + 10]->SetValue(10, 10, false);
	Nest[(10 * NESTSIZE) + 10]->HidePart();
	Nest[(10 * NESTSIZE) + 9]->SetValue(10, 9, false);
	Nest[(10 * NESTSIZE) + 9]->HidePart();
	Nest[(10 * NESTSIZE) + 8]->SetValue(10, 8, false);
	Nest[(10 * NESTSIZE) + 8]->HidePart();
	Nest[(10 * NESTSIZE) + 7]->SetValue(10, 7, false);
	Nest[(10 * NESTSIZE) + 7]->HidePart();

	Nest[(10 * NESTSIZE) + 5]->SetValue(10, 5, false);
	Nest[(10 * NESTSIZE) + 5]->HidePart();

	Nest[(10 * NESTSIZE) + 3]->SetValue(10, 3, false);
	Nest[(10 * NESTSIZE) + 3]->HidePart();
	Nest[(10 * NESTSIZE) + 2]->SetValue(10, 2, false);
	Nest[(10 * NESTSIZE) + 2]->HidePart();
	Nest[(10 * NESTSIZE) + 1]->SetValue(10, 1, false);
	Nest[(10 * NESTSIZE) + 1]->HidePart();

	Nest[(9 * NESTSIZE) + 13]->SetValue(9, 13, false);
	Nest[(9 * NESTSIZE) + 13]->HidePart();

	Nest[(9 * NESTSIZE) + 5]->SetValue(9, 5, false);
	Nest[(9 * NESTSIZE) + 5]->HidePart();

	Nest[(8 * NESTSIZE) + 13]->SetValue(8, 13, false);
	Nest[(8 * NESTSIZE) + 13]->HidePart();
	Nest[(8 * NESTSIZE) + 12]->SetValue(8, 12, false);
	Nest[(8 * NESTSIZE) + 12]->HidePart();
	Nest[(8 * NESTSIZE) + 11]->SetValue(8, 11, false);
	Nest[(8 * NESTSIZE) + 11]->HidePart();
	Nest[(8 * NESTSIZE) + 10]->SetValue(8, 10, false);
	Nest[(8 * NESTSIZE) + 10]->HidePart();

	Nest[(8 * NESTSIZE) + 7]->SetValue(8, 7, false);
	Nest[(8 * NESTSIZE) + 7]->HidePart();

	Nest[(8 * NESTSIZE) + 5]->SetValue(8, 5, false);
	Nest[(8 * NESTSIZE) + 5]->HidePart();
	Nest[(8 * NESTSIZE) + 4]->SetValue(8, 4, false);
	Nest[(8 * NESTSIZE) + 4]->HidePart();
	Nest[(8 * NESTSIZE) + 3]->SetValue(8, 3, false);
	Nest[(8 * NESTSIZE) + 3]->HidePart();
	Nest[(8 * NESTSIZE) + 2]->SetValue(8, 2, false);
	Nest[(8 * NESTSIZE) + 2]->HidePart();
	Nest[(8 * NESTSIZE) + 1]->SetValue(8, 1, false);
	Nest[(8 * NESTSIZE) + 1]->HidePart();

	Nest[(7 * NESTSIZE) + 11]->SetValue(7, 11, false);
	Nest[(7 * NESTSIZE) + 11]->HidePart();
	
	Nest[(7 * NESTSIZE) + 9]->SetValue(7, 9, false);
	Nest[(7 * NESTSIZE) + 9]->HidePart();
	Nest[(7 * NESTSIZE) + 8]->SetValue(7, 8, false);
	Nest[(7 * NESTSIZE) + 8]->HidePart();
	Nest[(7 * NESTSIZE) + 7]->SetValue(7, 7, false);
	Nest[(7 * NESTSIZE) + 7]->HidePart();
	
	Nest[(7 * NESTSIZE) + 5]->SetValue(7, 5, false);
	Nest[(7 * NESTSIZE) + 5]->HidePart();

	Nest[(7 * NESTSIZE) + 2]->SetValue(7, 2, false);
	Nest[(7 * NESTSIZE) + 2]->HidePart();

	Nest[(6 * NESTSIZE) + 13]->SetValue(6, 13, false);
	Nest[(6 * NESTSIZE) + 13]->HidePart();
	Nest[(6 * NESTSIZE) + 12]->SetValue(6, 12, false);
	Nest[(6 * NESTSIZE) + 12]->HidePart();
	Nest[(6 * NESTSIZE) + 11]->SetValue(6, 11, false);
	Nest[(6 * NESTSIZE) + 11]->HidePart();

	Nest[(6 * NESTSIZE) + 7]->SetValue(6, 7, false);
	Nest[(6 * NESTSIZE) + 7]->HidePart();

	Nest[(6 * NESTSIZE) + 5]->SetValue(6, 5, false);
	Nest[(6 * NESTSIZE) + 5]->HidePart();

	Nest[(5 * NESTSIZE) + 13]->SetValue(5, 13, false);
	Nest[(5 * NESTSIZE) + 13]->HidePart();

	Nest[(5 * NESTSIZE) + 11]->SetValue(5, 11, false);
	Nest[(5 * NESTSIZE) + 11]->HidePart();
	Nest[(5 * NESTSIZE) + 10]->SetValue(5, 10, false);
	Nest[(5 * NESTSIZE) + 10]->HidePart();
	Nest[(5 * NESTSIZE) + 9]->SetValue(5, 9, false);
	Nest[(5 * NESTSIZE) + 9]->HidePart();
	Nest[(5 * NESTSIZE) + 8]->SetValue(5, 8, false);
	Nest[(5 * NESTSIZE) + 8]->HidePart();
	Nest[(5 * NESTSIZE) + 7]->SetValue(5, 7, false);
	Nest[(5 * NESTSIZE) + 7]->HidePart();
	Nest[(5 * NESTSIZE) + 6]->SetValue(5, 6, false);
	Nest[(5 * NESTSIZE) + 6]->HidePart();
	Nest[(5 * NESTSIZE) + 5]->SetValue(5, 5, false);
	Nest[(5 * NESTSIZE) + 5]->HidePart();

	Nest[(5 * NESTSIZE) + 2]->SetValue(5, 2, false);
	Nest[(5 * NESTSIZE) + 2]->HidePart();
	Nest[(5 * NESTSIZE) + 1]->SetValue(5, 1, false);
	Nest[(5 * NESTSIZE) + 1]->HidePart();

	Nest[(4 * NESTSIZE) + 13]->SetValue(4, 13, false);
	Nest[(4 * NESTSIZE) + 13]->HidePart();

	Nest[(4 * NESTSIZE) + 5]->SetValue(4, 5, false);
	Nest[(4 * NESTSIZE) + 5]->HidePart();

	Nest[(4 * NESTSIZE) + 1]->SetValue(4, 1, false);
	Nest[(4 * NESTSIZE) + 1]->HidePart();

	Nest[(3 * NESTSIZE) + 13]->SetValue(3, 13, false);
	Nest[(3 * NESTSIZE) + 13]->HidePart();
	Nest[(3 * NESTSIZE) + 12]->SetValue(3, 12, false);
	Nest[(3 * NESTSIZE) + 12]->HidePart();
	Nest[(3 * NESTSIZE) + 11]->SetValue(3, 11, false);
	Nest[(3 * NESTSIZE) + 11]->HidePart();
	Nest[(3 * NESTSIZE) + 10]->SetValue(3, 10, false);
	Nest[(3 * NESTSIZE) + 10]->HidePart();
	Nest[(3 * NESTSIZE) + 9]->SetValue(3, 9, false);
	Nest[(3 * NESTSIZE) + 9]->HidePart();
	Nest[(3 * NESTSIZE) + 8]->SetValue(3, 8, false);
	Nest[(3 * NESTSIZE) + 8]->HidePart();
	Nest[(3 * NESTSIZE) + 7]->SetValue(3, 7, false);
	Nest[(3 * NESTSIZE) + 7]->HidePart();
	Nest[(3 * NESTSIZE) + 6]->SetValue(3, 6, false);
	Nest[(3 * NESTSIZE) + 6]->HidePart();
	Nest[(3 * NESTSIZE) + 5]->SetValue(3, 4, false);
	Nest[(3 * NESTSIZE) + 5]->HidePart();
	Nest[(3 * NESTSIZE) + 4]->SetValue(3, 4, false);
	Nest[(3 * NESTSIZE) + 4]->HidePart();
	Nest[(3 * NESTSIZE) + 3]->SetValue(3, 3, false);
	Nest[(3 * NESTSIZE) + 3]->HidePart();

	Nest[(3 * NESTSIZE) + 1]->SetValue(3, 1, false);
	Nest[(3 * NESTSIZE) + 1]->HidePart();

	Nest[(2 * NESTSIZE) + 11]->SetValue(2, 11, false);
	Nest[(2 * NESTSIZE) + 11]->HidePart();
	
	Nest[(2 * NESTSIZE) + 8]->SetValue(2, 8, false);
	Nest[(2 * NESTSIZE) + 8]->HidePart();
	
	Nest[(2 * NESTSIZE) + 5]->SetValue(2, 5, false);
	Nest[(2 * NESTSIZE) + 5]->HidePart();
	
	Nest[(2 * NESTSIZE) + 1]->SetValue(2, 1, false);
	Nest[(2 * NESTSIZE) + 1]->HidePart();

	Nest[(1 * NESTSIZE) + 13]->SetValue(1, 13, false);
	Nest[(1 * NESTSIZE) + 13]->HidePart();
	Nest[(1 * NESTSIZE) + 12]->SetValue(1, 12, false);
	Nest[(1 * NESTSIZE) + 12]->HidePart();
	Nest[(1 * NESTSIZE) + 11]->SetValue(1, 11, false);
	Nest[(1 * NESTSIZE) + 11]->HidePart();
	Nest[(1 * NESTSIZE) + 10]->SetValue(1, 10, false);
	Nest[(1 * NESTSIZE) + 10]->HidePart();
	Nest[(1 * NESTSIZE) + 9]->SetValue(1, 9, false);
	Nest[(1 * NESTSIZE) + 9]->HidePart();
	Nest[(1 * NESTSIZE) + 8]->SetValue(1, 8, false);
	Nest[(1 * NESTSIZE) + 8]->HidePart();
	Nest[(1 * NESTSIZE) + 7]->SetValue(1, 7, false);
	Nest[(1 * NESTSIZE) + 7]->HidePart();

	Nest[(1 * NESTSIZE) + 5]->SetValue(1, 5, false);
	Nest[(1 * NESTSIZE) + 5]->HidePart();
	Nest[(1 * NESTSIZE) + 4]->SetValue(1, 4, false);
	Nest[(1 * NESTSIZE) + 4]->HidePart();
	Nest[(1 * NESTSIZE) + 3]->SetValue(1, 3, false);
	Nest[(1 * NESTSIZE) + 3]->HidePart();
	Nest[(1 * NESTSIZE) + 2]->SetValue(1, 2, false);
	Nest[(1 * NESTSIZE) + 2]->HidePart();
	Nest[(1 * NESTSIZE) + 1]->SetValue(1, 1, false);
	Nest[(1 * NESTSIZE) + 1]->HidePart();

	ResourcesRoom = FVector(-350, 350, 50);
	NestRoom = FVector(350, 350, 50);
	DisposalRoom = FVector(350, -350, 50);
	QueenRoom = FVector(-350, -350, 50);

	// Delay
	GetWorld()->GetTimerManager().SetTimer(SeedSpawnHandler, this, &ANest::SpawnLandslip, FMath::RandRange(10.0f, 30.0f), false);
}

// Called every frame
void ANest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ANest::CheckHasPartsToDig()
{
	bool Has = false;

	for (int i = 0; i < NESTSIZE && !Has; i++)
	{
		for (int j = 0; j < NESTSIZE && !Has; j++)
		{
			Has = Has || (!Nest[(i * NESTSIZE) + j]->GetValue(i, j) && !Nest[(i * NESTSIZE) + j]->WasDig);
		}
	}

	if (!Has && !Started)
	{
		Started = true;
		OnFinishedDiggingDelegate.Broadcast();
	}

	return Has;
}

ANestPart* ANest::GetNextPartToDig(int32 &PartX, int32 &PartY, APawn* Ant)
{
	ANestPart* NextPart = nullptr;

	// Up part is valid to dig
	if ((((PartX + 1) * NESTSIZE) + PartY) >= 0 && (((PartX + 1) * NESTSIZE) + PartY) < (NESTSIZE * NESTSIZE) &&
		!Nest[((PartX + 1) * NESTSIZE) + PartY]->Value && !Nest[((PartX + 1) * NESTSIZE) + PartY]->WasDig)
	{
		PartX = PartX + 1;
		NextPart = Nest[(PartX * NESTSIZE) + PartY];
	}

	// Left part is valid to dig
	else if (((PartX * NESTSIZE) + (PartY - 1)) >= 0 && ((PartX * NESTSIZE) + (PartY - 1)) < (NESTSIZE * NESTSIZE) &&
		!Nest[(PartX * NESTSIZE) + (PartY - 1)]->Value && !Nest[(PartX * NESTSIZE) + (PartY - 1)]->WasDig)
	{
		PartY = PartY - 1;
		NextPart = Nest[(PartX * NESTSIZE) + PartY];
	}

	// Left part is valid to dig
	else if (((PartX * NESTSIZE) + (PartY + 1)) >= 0 && ((PartX * NESTSIZE) + (PartY + 1)) < (NESTSIZE * NESTSIZE) &&
		!Nest[(PartX * NESTSIZE) + (PartY + 1)]->Value && !Nest[(PartX * NESTSIZE) + (PartY + 1)]->WasDig)
	{
		PartY = PartY + 1;
		NextPart = Nest[(PartX * NESTSIZE) + PartY];
	}

	// Bottom part is valid to dig
	else if ((((PartX - 1) * NESTSIZE) + PartY) >= 0 && (((PartX - 1) * NESTSIZE) + PartY) < (NESTSIZE * NESTSIZE) &&
		!Nest[((PartX - 1) * NESTSIZE) + PartY]->Value && !Nest[((PartX - 1) * NESTSIZE) + PartY]->WasDig)
	{
		PartX = PartX - 1;
		NextPart = Nest[(PartX * NESTSIZE) + PartY];
	}

	else
	{ 
		float distance = 0;

		for (int i = NESTSIZE - 2; i > 0; i--)
		{
			for (int j = 1; j < NESTSIZE - 1; j++)
			{
				if (!Nest[(i * NESTSIZE) + j]->Value && !Nest[(i * NESTSIZE) + j]->WasDig && (Nest[((i + 1) * NESTSIZE) + j]->WasDig || Nest[(i * NESTSIZE) + (j - 1)]->WasDig
					|| Nest[(i * NESTSIZE) + (j + 1)]->WasDig || Nest[((i - 1) * NESTSIZE) + j]->WasDig))
				{
					PartX = i;
					PartY = j;

					if(NextPart)
					{
						if ((Ant->GetActorLocation() - Nest[(i * NESTSIZE) + j]->GetActorLocation()).Size() < distance)
						{
							NextPart = Nest[(PartX * NESTSIZE) + PartY];
							distance = (Ant->GetActorLocation() - Nest[(PartX * NESTSIZE) + PartY]->GetActorLocation()).Size();
						}
					}

					else
					{
						NextPart = Nest[(PartX * NESTSIZE) + PartY];
						distance = (Ant->GetActorLocation() - Nest[(PartX * NESTSIZE) + PartY]->GetActorLocation()).Size();
					}
				}
			}
		}
	}

	return NextPart;
}

ANestPart* ANest::GetFirstPartToDig(int32 &PartX, int32 &PartY)
{
	PartX = 14;
	PartY = 7;
	
	return  Nest[(PartX * NESTSIZE) + PartY];
}

void ANest::DigPart(int32 &PartX, int32 &PartY)
{
	Nest[((PartX - 1) * NESTSIZE) + PartY]->HidePart();
}

void ANest::SpawnLandslip()
{
	bool Found = false;

	int x = 0, y = 0;

	while (!Found)
	{
		x = FMath::RandRange(1, 13);
		y = FMath::RandRange(1, 13);

		Found = !Nest[(x * NESTSIZE) + y]->GetValue(x, y);
	}

	GetWorld()->SpawnActor<ALandslip>(LandslipClass, Nest[(x * NESTSIZE) + y]->GetActorLocation(), FRotator(0, 0, 0));

	// Delay
	GetWorld()->GetTimerManager().SetTimer(SeedSpawnHandler, this, &ANest::SpawnLandslip, FMath::RandRange(10.0f, 30.0f), false);
}