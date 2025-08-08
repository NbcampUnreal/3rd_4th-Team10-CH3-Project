#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveData.h"
#include "SpartaSpawnManager.generated.h"

UCLASS()
class TEAM10_PROJECT_API ASpartaSpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ASpartaSpawnManager();

    /** 스폰 시작 함수 - GameMode가 호출 */
    void StartSpawning(const FWaveData& WaveData);

protected:
    virtual void BeginPlay() override;

    /** 하나의 적을 스폰 */
    void SpawnNextEnemy();

protected:
    /** 적이 스폰될 위치들 */
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<AActor*> SpawnPoints;

    /** 타이머 핸들 */
    FTimerHandle SpawnTimerHandle;

    /** 현재 스폰 대기 중인 클래스들 */
    UPROPERTY()
    TArray<TSubclassOf<APawn>> PendingSpawnClasses;

    /** 클래스별 남은 스폰 수 */
    UPROPERTY()
    TArray<int32> PendingSpawnCounts;

    /** 현재 어떤 인덱스의 적을 스폰 중인지 */
    int32 CurrentSpawnIndex;
};
