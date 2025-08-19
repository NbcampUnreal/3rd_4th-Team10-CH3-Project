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

    // ==========================================================
    // 이 함수의 선언을 수정했습니다. (스폰 위치를 직접 받도록) 👇
    /** 스폰 시작 함수 - GameMode가 호출하며, 스폰할 위치를 지정해줍니다. */
    void StartSpawning(const FWaveData& WaveData, AActor* TargetSpawnPoint);
    // ==========================================================

protected:
    virtual void BeginPlay() override;
    void SpawnNextEnemy();

protected:
    // ==========================================================
    // 이 변수는 더 이상 스폰 매니저가 직접 관리할 필요가 없으므로 삭제했습니다.
    // UPROPERTY(EditAnywhere, Category = "Spawn")
    // TArray<AActor*> SpawnPoints;
    // ==========================================================

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

    /** 현재 웨이브에서 사용할 스폰 위치 (GameMode가 지정해 줌) */
    UPROPERTY()
    AActor* CurrentWaveSpawnPoint;
};