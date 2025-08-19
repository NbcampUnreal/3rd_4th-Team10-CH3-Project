#include "SpartaSpawnManager.h"
#include "SpartaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASpartaSpawnManager::ASpartaSpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentSpawnIndex = 0;
    CurrentWaveSpawnPoint = nullptr; // 멤버 변수 초기화
}

void ASpartaSpawnManager::BeginPlay()
{
    Super::BeginPlay();
}

void ASpartaSpawnManager::StartSpawning(const FWaveData& WaveData, AActor* TargetSpawnPoint)
{
    // 1. 게임모드가 지정해준 스폰 위치가 유효한지 확인합니다.
    if (!TargetSpawnPoint)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnManager received an invalid TargetSpawnPoint! Cannot start spawning."));
        return;
    }

    // 2. 이번 웨이브에서 사용할 스폰 위치로 전달받은 액터를 저장합니다.
    CurrentWaveSpawnPoint = TargetSpawnPoint;

    // 3. 스폰할 적 목록을 준비합니다.
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

    // 4. 스폰 타이머를 시작합니다.
    GetWorld()->GetTimerManager().SetTimer(
        SpawnTimerHandle, this, &ASpartaSpawnManager::SpawnNextEnemy, 0.3f, true);
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

    // 현재 웨이브의 스폰 위치가 유효하고, 스폰할 적 클래스가 있고, 남은 수가 0보다 큰지 확인합니다.
    if (EnemyClass && RemainingCount > 0 && CurrentWaveSpawnPoint)
    {
        // 랜덤 위치를 뽑는 대신, 저장해둔 CurrentWaveSpawnPoint를 사용합니다.
        FVector SpawnLocation = CurrentWaveSpawnPoint->GetActorLocation();
        FRotator SpawnRotation = CurrentWaveSpawnPoint->GetActorRotation();

        // 스폰 위치에 약간의 랜덤성을 추가합니다.
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

    // 현재 종류의 적을 모두 스폰했다면, 다음 종류의 적으로 넘어갑니다.
    if (RemainingCount <= 0)
    {
        CurrentSpawnIndex++;
    }
}