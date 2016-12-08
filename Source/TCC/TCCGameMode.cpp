// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "Engine.h"
#include "TCCGameMode.h"

ATCCGameMode::ATCCGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Character(TEXT("Blueprint'/Game/Blueprints/Characters/CharacterBlueprints/CameraActor/CharBP_Camera.CharBP_Camera'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> Controller(TEXT("Blueprint'/Game/Blueprints/PlCtrl_TCC.PlCtrl_TCC'"));
	
	DefaultPawnClass = (UClass*)Character.Object->GeneratedClass;
	PlayerControllerClass = (UClass*)Controller.Object->GeneratedClass;
}

void ATCCGameMode::StartPlay()
{
	Super::StartPlay();
}

