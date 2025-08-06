#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpartaGameMode.generated.h"

/**
 * SpartaGameMode
 * - 게임의 전체 진행을 관리하는 클래스
 * - 웨이브 시작/종료, 적 처치, 준비 시간 관리 등을 담당
 */
UCLASS()
class TEAM10_PROJECT_API ASpartaGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASpartaGameMode();

protected:
    virtual void BeginPlay() override;

    //=============================
    //  게임 흐름 (Game Flow)
    //=============================

    /** 웨이브 시작 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartWave();

    /** 웨이브 종료 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndWave();

    /** 적 처치 시 호출됨 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnEnemyKilled();

    /** 플레이어 사망 시 호출됨 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnPlayerDied();

    /** 게임 클리어 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnGameCleared();

    /** 게임 오버 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnGameOver();

    /** 웨이브 간 준비 시간 시작 */
    void StartInterWaveDelay();

    /** 준비 시간 카운트 다운 처리 (1초마다 호출) */
    void TickInterWaveDelay();

    //=============================
    //  웨이브 설정값 (Wave Settings)
    //=============================

    /** 최대 웨이브 수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Settings")
    int32 MaxWave;

    /** 기본 적 수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Settings")
    int32 BaseEnemyCount;

    /** 웨이브당 적 증가 계수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Settings")
    int32 WaveEnemyMultiplier;

    /** 웨이브 간 준비 시간 (초) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Settings")
    float InterWaveDelay;

    //=============================
    //  웨이브 상태값 (Wave Runtime)
    //=============================

    /** 현재 웨이브 번호 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    int32 WaveCounter;

    /** 현재 웨이브에 소환될 적 수 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    int32 EnemiesToSpawn;

    /** 남은 적 수 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    int32 EnemiesRemaining;

    /** 다음 웨이브 대기 상태 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    bool bWaitingForNextWave;

    /** 남은 준비 시간 (초) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|Runtime")
    int32 RemainingInterWaveTime;

    //=============================
    //  타이머 핸들
    //=============================

    /** 웨이브 시작 타이머 */
    FTimerHandle WaveStartTimerHandle;

    /** 준비 시간 카운트다운 타이머 */
    FTimerHandle InterWaveTickHandle;
};
