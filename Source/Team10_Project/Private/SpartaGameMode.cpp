#include "SpartaGameMode.h"
#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameMode::ASpartaGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

    WaveCounter = 0;
	MaxWave = 3;
	BaseEnemyCount = 3;
	WaveEnemyMultiplier = 2;
	WaveDelayTime = 5.0f;

	EnemiesToSpawn = 0;
	EnemiesRemaining = 0;
}

void ASpartaGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Game Start"));
	StartWave();
}

void ASpartaGameMode::StartWave()
{
    WaveCounter++;
	EnemiesToSpawn = BaseEnemyCount + WaveEnemyMultiplier * WaveCounter;
	EnemiesRemaining = EnemiesToSpawn;

    // GameState에 전달
    ASpartaGameState* GS = GetGameState<ASpartaGameState>();
    if (GS)
    {
        GS->SetDisplayWave(WaveCounter);
        GS->SetDisplayEnemiesRemaining(EnemiesRemaining);
    }

	// 이후에 SpawnManager가 생기면 여기서 적 스폰 호출
	// if (SpawnManager) { SpawnManager->SpawnEnemies(EnemiesToSpawn); }

	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start, Enemies %d"), WaveCounter, EnemiesToSpawn);
}

void ASpartaGameMode::EndWave()
{
	if (WaveCounter >= MaxWave)
	{
		OnGameCleared();
	}
	else
	{
		GetWorldTimerManager().SetTimer(
			WaveStartTimerHandle,
			this,
			&ASpartaGameMode::StartWave,
			WaveDelayTime,
			false
		);
	}
}

void ASpartaGameMode::OnEnemyKilled()
{
	EnemiesRemaining--;

    ASpartaGameState* GS = GetGameState<ASpartaGameState>();
    if (GS)
    {
        GS->AddScore(10);
        GS->DecreaseRemainingEnemies();
    }

	if (EnemiesRemaining <= 0)
	{
		EndWave();
	}
}

void ASpartaGameMode::OnGameCleared()
{
	UE_LOG(LogTemp, Warning, TEXT("GameClear!"));
}

void ASpartaGameMode::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("GameOver!"));
}
