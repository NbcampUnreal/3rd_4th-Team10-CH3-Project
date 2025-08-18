#include "SpartaGameMode.h"
#include "SpartaGameState.h"
#include "SpartaSpawnManager.h"
#include "WaveData.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameMode::ASpartaGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    WaveCounter = 0;
    bWaitingForNextWave = false;
    EnemiesRemaining = 0;
    InterWaveCountdown = 0.0f;
}

void ASpartaGameMode::BeginPlay()
{
    Super::BeginPlay();
    StartWave();
}

void ASpartaGameMode::StartWave()
{
    WaveCounter++;
    const int32 WaveIndex = WaveCounter - 1;

    if (!WaveData || !WaveData->Waves.IsValidIndex(WaveIndex))
    {
        OnGameCleared();
        return;
    }

    const FWaveData& CurrentWave = WaveData->Waves[WaveIndex];

    EnemiesRemaining = 0;
    for (const FEnemySpawnInfo& Info : CurrentWave.EnemiesToSpawn)
    {
        EnemiesRemaining += Info.SpawnCount;
    }

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayWave(WaveCounter);
        GS->SetDisplayEnemiesRemaining(EnemiesRemaining);
    }

    if (SpawnManagerRef)
    {
        SpawnManagerRef->StartSpawning(CurrentWave);
    }
}

void ASpartaGameMode::OnEnemyKilled(AActor* DestroyedActor)
{
    EnemiesRemaining--;

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayEnemiesRemaining(EnemiesRemaining);
    }

    if (EnemiesRemaining <= 0)
    {
        EndWave();
    }
}

void ASpartaGameMode::EndWave()
{
    const int32 WaveIndex = WaveCounter - 1;

    if (!WaveData || !WaveData->Waves.IsValidIndex(WaveIndex))
        return;

    const float Delay = WaveData->Waves[WaveIndex].TimeUntilNextWave;
    StartInterWaveDelay(Delay);
}

void ASpartaGameMode::StartInterWaveDelay(float InDelay)
{
    bWaitingForNextWave = true;
    InterWaveCountdown = InDelay;

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetWaitingWave(bWaitingForNextWave);
    }

    TickInterWaveDelay();

    GetWorldTimerManager().SetTimer(
        InterWaveTickHandle, this, &ASpartaGameMode::TickInterWaveDelay, 1.0f, true);
}

void ASpartaGameMode::TickInterWaveDelay()
{
    InterWaveCountdown--;

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayCountdown(InterWaveCountdown);
    }

    if (InterWaveCountdown <= 0)
    {
        GetWorldTimerManager().ClearTimer(InterWaveTickHandle);
        bWaitingForNextWave = false;
        if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
        {
            GS->SetWaitingWave(bWaitingForNextWave);
        }
        StartWave();
    }
}

void ASpartaGameMode::OnGameCleared()
{
    UE_LOG(LogTemp, Warning, TEXT("Game Cleared!"));
}

void ASpartaGameMode::OnPlayerDied()
{
    UE_LOG(LogTemp, Warning, TEXT("Player Died - Game Over"));
    OnGameOver();
}

void ASpartaGameMode::OnGameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}
