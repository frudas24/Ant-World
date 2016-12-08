// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "TCCPlayerController.h"

ATCCPlayerController::ATCCPlayerController()
{
	AntsQtt = 0;
	ProcessorQtt = 0;
	BuilderQtt = 0;
	FixerQtt = 0;
	CollectorQtt = 0;
	GuardQtt = 0;
	CleanerQtt = 0;
	Coins = 0;
	Period = "Collect";
	TimeRemaining = 0.0f;
	EnemiesLeft = 0;
	Turn = 0;
	ProcessorDead = 0;
	BuilderDead = 0;
	FixerDead = 0;
	CollectorDead = 0;
	GuardDead = 0;
	CleanerDead = 0;
}