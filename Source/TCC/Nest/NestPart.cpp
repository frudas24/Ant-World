// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "NestPart.h"

// Sets default values
ANestPart::ANestPart(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	RootComponent = Root;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> CubeMaterial(TEXT("Material'/Game/MobileStarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel'"));
	
	//SetRootComponent(Mesh);
	//RootComponent = Mesh;
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Cube"));
	Mesh->SetStaticMesh(CubeMesh.Object);
	Mesh->SetMaterial(0, CubeMaterial.Object);
	//Mesh->AttachParent = Root;
	Mesh->SetupAttachment(RootComponent);
}

ANestPart::ANestPart(int32 NewI, int32 NewJ, bool NewValue)
{
	PrimaryActorTick.bCanEverTick = false;
	i = NewI;
	j = NewJ;
	Value = NewValue;
	WasDig = false;
}

// Called when the game starts or when spawned
void ANestPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANestPart::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool ANestPart::GetValue(int32 NewI, int32 NewJ)
{
	bool aux = false;

	if (i == NewI && j == NewJ)
	{
		aux = Value;
	}

	return aux;
}

void ANestPart::SetValue(int32 NewI, int32 NewJ, bool NewValue)
{
	i = NewI;
	j = NewJ;
	Value = NewValue;
}

void ANestPart::SetLocation(int32 NewX, int32 NewY)
{
	SetActorLocation(FVector(NewX, NewY, 50));
}

void ANestPart::GetCoordinates(int32 &NewI, int32 &NewJ)
{
	NewI = i;
	NewJ = j;
}

void ANestPart::HidePart()
{
	//Mesh->bCanEverAffectNavigation = false;
	Mesh->SetCanEverAffectNavigation(false);
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WasDig = true;
}