#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpartaGameMode.generated.h"

/**
 * ASpartaGameMode
 * - 웨이브 기반 게임의 흐름을 제어하는 메인 클래스
 * - 웨이브 시작/종료, 적 처치, 게임 승리/패배 처리 등 담당
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
    //  게임 진행 함수 (Game Flow)
    //=============================

    /** 웨이브 시작 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartWave();

    /** 웨이브 종료 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndWave();

    /** 적 처치 시 호출 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnEnemyKilled();

    /** 게임 승리 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnGameCleared();

    /** 게임 오버 처리 */
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void OnGameOver();

    //=============================
    //  웨이브 설정값 (Wave Settings)
    //=============================

    /** 현재 웨이브 번호 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
    int32 WaveCounter;

    /** 최대 웨이브 수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
    int32 MaxWave;

    /** 웨이브 시작 시 적 기본 수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
    int32 BaseEnemyCount;

    /** 웨이브마다 증가하는 적 수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
    int32 WaveEnemyMultiplier;

    /** 웨이브 간 딜레이 시간 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
    float WaveDelayTime;

    //=============================
    //  웨이브 상태값 (Wave Runtime)
    //=============================

    /** 현재 웨이브에 스폰될 적 수 */
    UPROPERTY(BlueprintReadOnly, Category = "Wave State")
    int32 EnemiesToSpawn;

    /** 현재 남아있는 적 수 */
    UPROPERTY(BlueprintReadOnly, Category = "Wave State")
    int32 EnemiesRemaining;

    //=============================
    //  타이머 (Wave Timer)
    //=============================

    /** 다음 웨이브를 위한 타이머 핸들 */
    FTimerHandle WaveStartTimerHandle;
};
