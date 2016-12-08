// Fill out your copyright notice in the Description page of Project Settings.

#include "TCC.h"
#include "EnemySpawn.h"
#include "Characters/Enemy.h"
#include "AI/AICtrl_Enemy.h"
#include "Resources/WaypointSystem.h"
#include "Resources/ProcessedSeed.h"
#include "Resources/Seed.h"

// rtNeat Includes
#include "rtNEAT/Organism.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> EnemyBlueprint(TEXT("Blueprint'/Game/Blueprints/Characters/CharacterBlueprints/Ants/CharBP_Enemy.CharBP_Enemy'"));

	if (EnemyBlueprint.Object)
	{
		EnemyClass = (UClass*)EnemyBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AWaypointSystem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AWaypointSystem* WaypointSystem = *ActorItr;

		if (WaypointSystem->Type == 1)
		{
			SpawnLocation = WaypointSystem->Waypoints.Last(0);
		}
	}
}

// Called every frame
void AEnemySpawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEnemySpawn::LoadNeatParams()
{
	NEAT::trait_param_mut_prob = 0.5;
	NEAT::trait_mutation_power = 1.0; // Power of mutation on a signle trait param 
	NEAT::linktrait_mut_sig = 1.0; // Amount that mutation_num changes for a trait change inside a link
	NEAT::nodetrait_mut_sig = 0.5; // Amount a mutation_num changes on a link connecting a node that changed its trait 
	NEAT::weight_mut_power = 2.5; // The power of a linkweight mutation 
	NEAT::recur_prob = 0.00; // Prob. that a link mutation which doesn't have to be recurrent will be made recurrent 
	NEAT::disjoint_coeff = 1.0;
	NEAT::excess_coeff = 1.0;
	NEAT::mutdiff_coeff = 0.4;
	NEAT::compat_threshold = 3.0;
	NEAT::age_significance = 1.0; // How much does age matter? 
	NEAT::survival_thresh = 0.20; // Percent of ave fitness for survival 
	NEAT::mutate_only_prob = 0.25; // Prob. of a non-mating reproduction 
	NEAT::mutate_random_trait_prob = 0.1;
	NEAT::mutate_link_trait_prob = 0.1;
	NEAT::mutate_node_trait_prob = 0.1;
	NEAT::mutate_link_weights_prob = 0.9;
	NEAT::mutate_toggle_enable_prob = 0.00;
	NEAT::mutate_gene_reenable_prob = 0.000;
	NEAT::mutate_add_node_prob = 0.03;
	NEAT::mutate_add_link_prob = 0.05;
	NEAT::interspecies_mate_rate = 0.001; // Prob. of a mate being outside species 
	NEAT::mate_multipoint_prob = 0.6;
	NEAT::mate_multipoint_avg_prob = 0.4;
	NEAT::mate_singlepoint_prob = 0.0;
	NEAT::mate_only_prob = 0.2; // Prob. of mating without mutation 
	NEAT::recur_only_prob = 0.0;  // Probability of forcing selection of ONLY links that are naturally recurrent 
	NEAT::pop_size = 150;  // Size of population 
	NEAT::dropoff_age = 15;  // Age where Species starts to be penalized 
	NEAT::newlink_tries = 20;  // Number of tries mutate_add_link will attempt to find an open link 
	NEAT::print_every = 5; // Tells to print population to file every n generations 
	NEAT::babies_stolen = 0; // The number of babies to siphen off to the champions 
	NEAT::num_runs = 1;
	Inputs = 5;
	Outputs = 6;
}

NEAT::Population* Pop;
int Generation;
std::string NowDateTime;
std::ofstream File;

/*void AEnemySpawn::CreateFirstGeneration()
{
	LoadNeatParams();

	NEAT::pop_size = 5;

	Generation = 1;

	Pop = new NEAT::Population(NEAT::pop_size, Inputs, Outputs, 10, false, 0.3);
	Pop->Verify();

	float outputs[5][4];

	std::vector<NEAT::Organism*>::iterator curorg;
	std::vector<NEAT::NNode*>::iterator curnode;
	int i = 0, j = 0;

	for (i = 0, curorg = (Pop->organisms).begin(); curorg != (Pop->organisms).end(); ++curorg, i++)
	{
		std::vector<float> in = { 0, 1 / 3, 1 / 3, 0, 1 / 3 };

		(*curorg)->net->load_sensors(in);

		bool success = (*curorg)->net->activate();

		for (j = 0, curnode = (*curorg)->net->outputs.begin(); curnode != (*curorg)->net->outputs.end(); ++curnode, j++)
		{
			outputs[i][j] = (*curnode)->activation;
		}
	}

	/*NEAT::Genome* Gen = new NEAT::Genome(0, 7, 1, FMath::RandRange(0, 10), 10, false, 0.3);

	NEAT::Network* Net = Gen->genesis(0);

	NEAT::Organism* Org = new NEAT::Organism(0, Gen, 1, nullptr);
}*/

AEnemy* AEnemySpawn::SpawnEnemy()
{
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator(0, 0, 0));
	Enemies.Add(NewEnemy);
	NewEnemy->SpawnDefaultController();

	ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Controller)
	{
		Controller->EnemiesLeft++;
	}

	return NewEnemy;
}

TArray<bool> AEnemySpawn::CreateFirstGeneration(bool CanSaveFile, int32 AntsQtt, int32 ProcessorQtt, int32 BuilderQtt, int32 CollectorQtt, int32 GuardQtt, int32 CleanerQtt)
{
	// Load default NEAT parameters
	LoadNeatParams();

	// Set new population size
	NEAT::pop_size = FMath::Clamp(AntsQtt, 1, 1000);

	// First generation
	Generation = 1;

	// Create population
	Pop = new NEAT::Population(NEAT::pop_size, Inputs, Outputs, 30, false, 0.3);
	Pop->Verify();

	// Iterators
	std::vector<NEAT::Organism*>::iterator curorg;
	std::vector<NEAT::NNode*>::iterator curnode;
	int i = 0, j = 0;

	// In Array
	std::vector<float> in = { (float)(GuardQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(CollectorQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(BuilderQtt / FMath::Clamp(AntsQtt, 1, 1000)),
		(float)(CleanerQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(ProcessorQtt / FMath::Clamp(AntsQtt, 1, 1000)) };
	
	TArray<bool> ReturnArray;

	Enemies.Empty();

	if (CanSaveFile)
	{
		std::string TmpTime(TCHAR_TO_UTF8(*FDateTime::Now().ToString()));
		NowDateTime = TmpTime;

		// Start first turn in file
		File.open("SaveValues" + NowDateTime + ".txt");
		File << "Rodada " << Generation << std::endl;

		File << "ENTRADAS" << std::endl;
		File << "Patrulheiras " << GuardQtt << "/" << AntsQtt << std::endl;
		File << "Coletoras " << CollectorQtt << "/" << AntsQtt << std::endl;
		File << "Construtoras " << BuilderQtt << "/" << AntsQtt << std::endl;
		File << "Lixeiras " << CleanerQtt << "/" << AntsQtt << std::endl;
		File << "Processadoras " << ProcessorQtt << "/" << AntsQtt << std::endl;

		File << std::endl << "SAIDAS" << std::endl;
	}

	// Run first NEAT
	for (i = 0, curorg = (Pop->organisms).begin(); curorg != (Pop->organisms).end(); ++curorg, i++)
	{
		(*curorg)->net->load_sensors(in);

		bool success = (*curorg)->net->activate();

		AEnemy* Enemy = SpawnEnemy();
		Enemy->OnDiedDelegate.AddDynamic(this, &AEnemySpawn::OnEnemyDied);

		if (CanSaveFile)
		{
			std::string EnemyNameStr(TCHAR_TO_UTF8(*Enemy->GetName()));
			File << std::endl << EnemyNameStr.c_str() << std::endl;
		}

		for (j = 0, curnode = (*curorg)->net->outputs.begin(); curnode != (*curorg)->net->outputs.end(); ++curnode, j++)
		{
			ReturnArray.Add((*curnode)->activation > 0.5f);
		}
		
		AAICtrl_Enemy* EnemyController = Cast<AAICtrl_Enemy>(Enemy->GetController());

		if (EnemyController)
		{
			EnemyController->OnRetreatCompletedDelegate.AddDynamic(this, &AEnemySpawn::OnEnemyRetreat);
			EnemyController->SetOutputValues(ReturnArray[((i * Outputs) + j) - 6], ReturnArray[((i * Outputs) + j) - 5], ReturnArray[((i * Outputs) + j) - 4], ReturnArray[((i * Outputs) + j) - 3], ReturnArray[((i * Outputs) + j) - 2], ReturnArray[((i * Outputs) + j) - 1]);
			
			if (CanSaveFile)
			{
				File << ((ReturnArray[((i * Outputs) + j) - 6]) ? "Atacar" : "Roubar") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 5]) ? "Pode atacar junto" : "Só ataca sozinho") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 4]) ? "Flanquear" : "Direto para entrada") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 3]) ? "Anda fora do formigueiro" : "Anda dentro do formigueiro") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 2]) ? "Mais dano" : "Menos dano") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 1]) ? "Menos intervalo" : "Mais intervalo") << std::endl;
			}

			Attack.Insert(EnemyController->Attack, i);
		}
	}

	if (CanSaveFile)
	{
		File.close();

		// Print Population Net to file
		std::string FileName = "Population" + NowDateTime + ".txt";
		char *CharFileName = new char[FileName.length() + 1];
		strcpy(CharFileName, FileName.c_str());

		Pop->print_to_file_by_species(CharFileName);

		delete[] CharFileName;
	}

	return ReturnArray;
}

TArray<bool> AEnemySpawn::CreateGenerationFromFile(bool CanSaveFile, FString FileName, int32 AntsQtt, int32 ProcessorQtt, int32 BuilderQtt, int32 CollectorQtt, int32 GuardQtt, int32 CleanerQtt)
{
	// Load default NEAT parameters
	LoadNeatParams();

	// Set new population size
	//NEAT::pop_size = FMath::Clamp(AntsQtt, 1, 1000);

	// First generation
	Generation = 1;

	// Create population
	Pop = new NEAT::Population(TCHAR_TO_UTF8(*FileName));
	Pop->Verify();
	
	// Iterators
	std::vector<NEAT::Organism*>::iterator curorg;
	std::vector<NEAT::NNode*>::iterator curnode;
	int i = 0, j = 0;

	// In Array
	std::vector<float> in = { (float)(GuardQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(CollectorQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(BuilderQtt / FMath::Clamp(AntsQtt, 1, 1000)),
		(float)(CleanerQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(ProcessorQtt / FMath::Clamp(AntsQtt, 1, 1000)) };

	TArray<bool> ReturnArray;

	Enemies.Empty();

	if (CanSaveFile)
	{
		std::string TmpTime(TCHAR_TO_UTF8(*FDateTime::Now().ToString()));
		NowDateTime = TmpTime;

		// Start first turn in file
		File.open("SaveValues" + NowDateTime + ".txt");
		File << "Rodada " << Generation << std::endl;

		File << "ENTRADAS" << std::endl;
		File << "Patrulheiras " << GuardQtt << "/" << AntsQtt << std::endl;
		File << "Coletoras " << CollectorQtt << "/" << AntsQtt << std::endl;
		File << "Construtoras " << BuilderQtt << "/" << AntsQtt << std::endl;
		File << "Lixeiras " << CleanerQtt << "/" << AntsQtt << std::endl;
		File << "Processadoras " << ProcessorQtt << "/" << AntsQtt << std::endl;

		File << std::endl << "SAIDAS" << std::endl;
	}

	// Run first NEAT
	for (i = 0, curorg = (Pop->organisms).begin(); curorg != (Pop->organisms).end(); ++curorg, i++)
	{
		(*curorg)->net->load_sensors(in);

		bool success = (*curorg)->net->activate();

		AEnemy* Enemy = SpawnEnemy();
		Enemy->OnDiedDelegate.AddDynamic(this, &AEnemySpawn::OnEnemyDied);

		if (CanSaveFile)
		{
			std::string EnemyNameStr(TCHAR_TO_UTF8(*Enemy->GetName()));
			File << std::endl << EnemyNameStr.c_str() << std::endl;
		}

		for (j = 0, curnode = (*curorg)->net->outputs.begin(); curnode != (*curorg)->net->outputs.end(); ++curnode, j++)
		{
			ReturnArray.Add((*curnode)->activation > 0.5f);
		}

		AAICtrl_Enemy* EnemyController = Cast<AAICtrl_Enemy>(Enemy->GetController());

		if (EnemyController)
		{
			EnemyController->OnRetreatCompletedDelegate.AddDynamic(this, &AEnemySpawn::OnEnemyRetreat);
			EnemyController->SetOutputValues(ReturnArray[((i * Outputs) + j) - 6], ReturnArray[((i * Outputs) + j) - 5], ReturnArray[((i * Outputs) + j) - 4], ReturnArray[((i * Outputs) + j) - 3], ReturnArray[((i * Outputs) + j) - 2], ReturnArray[((i * Outputs) + j) - 1]);
			
			if (CanSaveFile)
			{
				File << ((ReturnArray[((i * Outputs) + j) - 6]) ? "Atacar" : "Roubar") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 5]) ? "Pode atacar junto" : "Só ataca sozinho") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 4]) ? "Flanquear" : "Direto para entrada") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 3]) ? "Anda fora do formigueiro" : "Anda dentro do formigueiro") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 2]) ? "Mais dano" : "Menos dano") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 1]) ? "Menos intervalo" : "Mais intervalo") << std::endl;
			}
			Attack.Insert(EnemyController->Attack, i);
		}
	}

	if (CanSaveFile)
	{
		File.close();

		// Print Population Net to file
		std::string FileName = "Population" + NowDateTime + ".txt";
		char *CharFileName = new char[FileName.length() + 1];
		strcpy(CharFileName, FileName.c_str());

		Pop->print_to_file_by_species(CharFileName);

		delete[] CharFileName;
	}

	return ReturnArray;
}

float AEnemySpawn::CalculateFitness(bool Attack, int32 DamageGiven, int32 AntsFought, int32 HealthLeft, int32 SeedsStolen, int32 AntsQtt)
{	
	//return (AntsFought != 0) ? (DamageGiven / (AntsFought * 100)) + (HealthLeft / 100) : (HealthLeft / 100);
	//return Attack ? ((double)DamageGiven / ((double)(AntsFought == 0 ? 1.0 : AntsFought * 100))) + ((double)HealthLeft / 100) + ((double)SeedsStolen * 0.1f) : ((double)DamageGiven / ((double)(AntsFought == 0 ? 1.0 : AntsFought * 1000))) + ((double)HealthLeft / 100) + ((double)SeedsStolen * 0.1f);
	
	int32 ResourcesAvailable = 0;

	for (TActorIterator<AProcessedSeed> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AProcessedSeed* Seed = *ActorItr;

		if (!Seed->WasStolen)
		{
			ResourcesAvailable++;
		}
	}

	for (TActorIterator<ASeed> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ASeed* Seed = *ActorItr;

		if (Seed)
		{
			ResourcesAvailable++;
		}
	}

	return (DamageGiven - (100 - HealthLeft)) + ((AntsQtt + 5 - ResourcesAvailable) * SeedsStolen * 10);
}

void AEnemySpawn::DeletePopulation()
{
	delete Pop;
}

TArray<bool> AEnemySpawn::Evolve(bool CanSaveFile, int32 AntsQtt, int32 ProcessorQtt, int32 BuilderQtt, int32 CollectorQtt, int32 GuardQtt, int32 CleanerQtt)
{
	Generation++;

	// Set new population size
	NEAT::pop_size = FMath::Clamp(AntsQtt, 1, 1000);

	// Iterators
	std::vector<NEAT::Organism*>::iterator curorg;
	std::vector<NEAT::NNode*>::iterator curnode;
	int i = 0, j = 0;

	// In Array
	std::vector<float> in = { (float)(GuardQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(CollectorQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(BuilderQtt / FMath::Clamp(AntsQtt, 1, 1000)),
		(float)(CleanerQtt / FMath::Clamp(AntsQtt, 1, 1000)), (float)(ProcessorQtt / FMath::Clamp(AntsQtt, 1, 1000)) };

	// Calculate fitness
	for (i = 0, curorg = (Pop->organisms).begin(); curorg != (Pop->organisms).end() && i < NEAT::pop_size; ++curorg, i++)
	{
		(*curorg)->fitness = CalculateFitness(Attack[i], DamageGiven[i], AntsFought[i], HealthLeft[i], SeedsStolen[i], AntsQtt);
	}
	
	// Remove worst organism
	NEAT::Organism* tmp = Pop->remove_worst();
	
	// Insert new best fit
	tmp = Pop->reproduce_champ(Generation - 1);

	// Insert new best fit if the player bought new ants
	while (Pop->organisms.size() < NEAT::pop_size)
	{
		Pop->organisms.push_back(Pop->reproduce_champ(Generation - 1));
	}

	// Sort population
	Pop->rank_within_species();

	TArray<bool> ReturnArray;

	Enemies.Empty();

	if (CanSaveFile)
	{
		// Start new turn in file
		File.open("SaveValues" + NowDateTime + ".txt", std::ofstream::app);
		File << std::endl << std::endl;
		File << "Rodada " << Generation << std::endl;

		File << "ENTRADAS" << std::endl;
		File << "Patrulheiras " << GuardQtt << "/" << AntsQtt << std::endl;
		File << "Coletoras " << CollectorQtt << "/" << AntsQtt << std::endl;
		File << "Construtoras " << BuilderQtt << "/" << AntsQtt << std::endl;
		File << "Lixeiras " << CleanerQtt << "/" << AntsQtt << std::endl;
		File << "Processadoras " << ProcessorQtt << "/" << AntsQtt << std::endl;

		File << std::endl  << "SAIDAS" << std::endl;
	}

	// Spawn evolved enemies
	for (i = 0, curorg = (Pop->organisms).begin(); curorg != (Pop->organisms).end() && i < NEAT::pop_size; ++curorg, i++)
	{
		(*curorg)->net->load_sensors(in);

		bool success = (*curorg)->net->activate();
		
		AEnemy* Enemy = SpawnEnemy();
		Enemy->OnDiedDelegate.AddDynamic(this, &AEnemySpawn::OnEnemyDied);

		if (CanSaveFile)
		{
			std::string EnemyNameStr(TCHAR_TO_UTF8(*Enemy->GetName()));
			File << std::endl << EnemyNameStr.c_str() << std::endl;
		}

		for (j = 0, curnode = (*curorg)->net->outputs.begin(); curnode != (*curorg)->net->outputs.end(); ++curnode, j++)
		{
			ReturnArray.Add((*curnode)->activation > 0.5f);
		}

		AAICtrl_Enemy* EnemyController = Cast<AAICtrl_Enemy>(Enemy->GetController());

		// Set neural network output values in BTree
		if (EnemyController)
		{
			EnemyController->OnRetreatCompletedDelegate.AddDynamic(this, &AEnemySpawn::OnEnemyRetreat);
			EnemyController->SetOutputValues(ReturnArray[((i * Outputs) + j) - 6], ReturnArray[((i * Outputs) + j) - 5], ReturnArray[((i * Outputs) + j) - 4], ReturnArray[((i * Outputs) + j) - 3], ReturnArray[((i * Outputs) + j) - 2], ReturnArray[((i * Outputs) + j) - 1]);

			if (CanSaveFile)
			{
				// Print outputs to file
				File << ((ReturnArray[((i * Outputs) + j) - 6]) ? "Atacar" : "Roubar") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 5]) ? "Pode atacar junto" : "Só ataca sozinho") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 4]) ? "Flanquear" : "Direto para entrada") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 3]) ? "Anda fora do formigueiro" : "Anda dentro do formigueiro") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 2]) ? "Mais dano" : "Menos dano") << std::endl;
				File << ((ReturnArray[((i * Outputs) + j) - 1]) ? "Menos intervalo" : "Mais intervalo") << std::endl;
			}

			Attack.Insert(EnemyController->Attack, i);
		}
	}
	if (CanSaveFile)
	{
		File.close();

		// Print Population Net to file
		std::string FileName = "Population" + NowDateTime + ".txt";
		char *CharFileName = new char[FileName.length() + 1];
		strcpy(CharFileName, FileName.c_str());

		Pop->print_to_file_by_species(CharFileName);

		delete[] CharFileName;
	}

	return ReturnArray;
}

void AEnemySpawn::Retreat(bool CanSaveFile)
{
	if (CanSaveFile)
	{
		File.open("SaveValues" + NowDateTime + ".txt", std::ofstream::app);
		File << std::endl << std::endl;
		File << "Fim da Rodada " << Generation << std::endl;
	}

	for (int32 i = 0; i < Enemies.Num(); i++)
	{
		AEnemy* Enemy = Enemies[i];
		
		if (Enemy && !Enemy->IsDead())
		{
			if (CanSaveFile)
			{
				// Print results
				std::string EnemyNameStr(TCHAR_TO_UTF8(*Enemy->GetName()));
				File << std::endl << EnemyNameStr.c_str() << std::endl;

				File << "Dano dado " << Enemy->DamageGiven << std::endl;
				File << "Vida restante " << FMath::Clamp(Enemy->Health, 0, 100) << std::endl;
				File << "Lutou contra " << Enemy->AntsFought << " formigas" << std::endl;
				File << "Roubou " << Enemy->SeedsStolen << " sementes" << std::endl;
			}

			AAICtrl_Enemy* EnemyController = Cast<AAICtrl_Enemy>(Enemy->GetController());

			if (EnemyController)
			{
				EnemyController->Retreat(SpawnLocation, i);

				DamageGiven.Insert(Enemy->DamageGiven, i);
				HealthLeft.Insert(FMath::Clamp(Enemy->Health, 0 , 100), i);
				AntsFought.Insert(Enemy->AntsFought, i);
				SeedsStolen.Insert(Enemy->SeedsStolen, i);
			}
		}

		else if (Enemy && Enemy->IsDead())
		{
			if (CanSaveFile)
			{
				// Print results
				std::string EnemyNameStr(TCHAR_TO_UTF8(*Enemy->GetName()));
				File << std::endl << EnemyNameStr.c_str() << std::endl;

				File << "Dano dado " << Enemy->DamageGiven << std::endl;
				File << "Vida restante " << FMath::Clamp(Enemy->Health, 0, 100) << std::endl;
				File << "Lutou contra " << Enemy->AntsFought << " formigas" << std::endl;
				File << "Roubou " << Enemy->SeedsStolen << " sementes" << std::endl;
			}

			DamageGiven.Insert(Enemy->DamageGiven, i);
			HealthLeft.Insert(FMath::Clamp(Enemy->Health, 0, 100), i);
			AntsFought.Insert(Enemy->AntsFought, i);
			SeedsStolen.Insert(Enemy->SeedsStolen, i);

			Enemies[i] = nullptr;
			Enemy->GetController()->UnPossess();
			Enemy->Destroy();
		}
	}

	if (CanSaveFile)
	{
		File.close();
	}
}

void AEnemySpawn::OnEnemyDied(ABaseCharacter* DeadCharacter)
{
	ATCCPlayerController* Controller = Cast<ATCCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Controller)
	{
		Controller->Coins++;
		Controller->EnemiesLeft--;
	}
}

void AEnemySpawn::OnEnemyRetreat(ABaseCharacter* Character, int32 Index)
{
	Enemies[Index] = nullptr;
	Character->GetController()->UnPossess();
	Character->Destroy();
}