#include "SpartaSpawnManager.h"
#include "SpartaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASpartaSpawnManager::ASpartaSpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentSpawnIndex = 0;
}

void ASpartaSpawnManager::BeginPlay()
{
    Super::BeginPlay();
}

void ASpartaSpawnManager::StartSpawning(const FWaveData& WaveData)
{
    PendingSpawnClasses.Empty();
    PendingSpawnCounts.Empty();
    CurrentSpawnIndex = 0;

    for (const FEnemySpawnInfo& Info : WaveData.EnemiesToSpawn)
    {
        if (Info.EnemyClass)
        {
            PendingSpawnClasses.Add(Info.EnemyClass);
            PendingSpawnCounts.Add(Info.SpawnCount);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &ASpartaSpawnManager::SpawnNextEnemy,
        0.3f, // spawn interval
        true
    );
}

void ASpartaSpawnManager::SpawnNextEnemy()
{
    if (CurrentSpawnIndex >= PendingSpawnClasses.Num())
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    TSubclassOf<APawn> EnemyClass = PendingSpawnClasses[CurrentSpawnIndex];
    int32& RemainingCount = PendingSpawnCounts[CurrentSpawnIndex];

    if (EnemyClass && RemainingCount > 0 && SpawnPoints.Num() > 0)
    {
        // 랜덤 스폰 위치
        AActor* SpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
        FVector SpawnLocation = SpawnPoint->GetActorLocation();
        FRotator SpawnRotation = SpawnPoint->GetActorRotation();

        SpawnLocation += FVector(
            FMath::FRandRange(-100.f, 100.f),
            FMath::FRandRange(-100.f, 100.f),
            0.0f
        );

        APawn* SpawnedEnemy = GetWorld()->SpawnActor<APawn>(EnemyClass, SpawnLocation, SpawnRotation);

        if (SpawnedEnemy)
        {
            // GameMode에 적이 죽었을 때 알릴 수 있도록 바인딩
            ASpartaGameMode* GM = Cast<ASpartaGameMode>(UGameplayStatics::GetGameMode(this));
            if (GM)
            {
                SpawnedEnemy->OnDestroyed.AddDynamic(GM, &ASpartaGameMode::OnEnemyKilled);
            }

            RemainingCount--;
        }
    }

    // 다음 적으로 넘어갈 준비
    if (RemainingCount <= 0)
    {
        CurrentSpawnIndex++;
    }
}
