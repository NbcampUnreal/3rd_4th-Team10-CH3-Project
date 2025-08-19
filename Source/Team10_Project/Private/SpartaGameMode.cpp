#include "SpartaGameMode.h"
#include "SpartaGameState.h"
#include "SpartaSpawnManager.h"
#include "WaveData.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameMode::ASpartaGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    WaveCounter = 0;
    UE_LOG(LogTemp, Error, TEXT("SpartaGameMode C++ CONSTRUCTOR IS RUNNING!"));
    bWaitingForNextWave = false;
    EnemiesRemaining = 0;
    InterWaveCountdown = 0.0f;
}

void ASpartaGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Error, TEXT("SpartaGameMode C++ BeginPlay IS RUNNING!"));
    SpawnManagerRef = Cast<ASpartaSpawnManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ASpartaSpawnManager::StaticClass())
    );

   
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
        // 1. 웨이브 데이터에서 사용할 '태그'를 가져옵니다.
        FName DesiredTag = CurrentWave.SpawnPointTag;

        AActor* FoundSpawnPoint = nullptr;

        // 태그가 비어있지 않은지 확인합니다.
        if (!DesiredTag.IsNone())
        {
            // 2. 월드에서 해당 태그를 가진 모든 액터를 찾습니다.
            TArray<AActor*> ActorsWithTag;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), DesiredTag, ActorsWithTag);

            if (ActorsWithTag.Num() > 0)
            {
                // 3. 태그를 가진 액터를 찾았다면, 첫 번째 액터를 스폰 위치로 지정합니다.
                FoundSpawnPoint = ActorsWithTag[0];
            }
        }

        // 4. 스폰 위치를 성공적으로 찾았을 경우에만 스폰을 시작하도록 명령합니다.
        if (FoundSpawnPoint)
        {
            UE_LOG(LogTemp, Log, TEXT("Wave %d starting at spawn point with tag '%s'"), WaveCounter, *DesiredTag.ToString());

            // 수정된 StartSpawning 함수에 두 번째 인수로 찾은 스폰 위치를 함께 전달합니다.
            SpawnManagerRef->StartSpawning(CurrentWave, FoundSpawnPoint);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find Actor with tag '%s' for Wave %d!"), *DesiredTag.ToString(), WaveCounter);
        }
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