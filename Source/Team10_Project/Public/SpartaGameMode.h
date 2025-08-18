#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpartaGameMode.generated.h"

class ASpartaSpawnManager;
class UWaveDataAsset;

/**
 * ASpartaGameMode
 * - 게임의 웨이브 흐름을 관리하는 메인 클래스
 * - 적 스폰, 웨이브 시작/종료, 대기 시간 관리
 */
UCLASS()
class TEAM10_PROJECT_API ASpartaGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASpartaGameMode();

protected:
    virtual void BeginPlay() override;

    void StartWave();
    void EndWave();
    void OnGameCleared();
    void OnGameOver();

    UFUNCTION()
    void OnPlayerDied();

    void StartInterWaveDelay(float InDelay);
    void TickInterWaveDelay();

public:
    UFUNCTION()
    void OnEnemyKilled(AActor* DestroyedActor);

protected:
    /** 현재 웨이브 번호 (1부터 시작) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    int32 WaveCounter;

    /** 현재 웨이브의 남은 적 수 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    int32 EnemiesRemaining;

    /** 웨이브 간 대기 시간 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    float InterWaveCountdown;

    /** 웨이브 대기 중인지 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    bool bWaitingForNextWave;

    /** 웨이브 데이터 에셋 */
    UPROPERTY(VisibleAnywhere, Category = "Wave|Settings")
    UWaveDataAsset* WaveData;

    /** 스폰 매니저 참조 (월드에서 설정) */
    UPROPERTY(VisibleAnywhere, Category = "Wave|Settings")
    ASpartaSpawnManager* SpawnManagerRef;

    UPROPERTY(VisibleAnywhere, Category = "Wave|Settings")
    TSubclassOf<ASpartaSpawnManager> SpawnManagerClass;

    /** 대기 시간용 타이머 핸들 */
    FTimerHandle InterWaveTickHandle;
};
