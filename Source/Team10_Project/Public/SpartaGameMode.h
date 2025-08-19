#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"            // [Added] 위젯 사용
#include "SpartaGameMode.generated.h"

class ASpartaSpawnManager;
class UWaveDataAsset;

// [Added] 게임 종료 사유
UENUM(BlueprintType)
enum class EEndReason : uint8
{
    Cleared,
    GameOver
};

// [Added] HUD/블루프린트에서 바인딩 가능한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnded, EEndReason, Reason);

/**
 * ASpartaGameMode
 * - 웨이브 흐름, 적 스폰, 대기 시간, 점수, 종료 시 UI 트리거 관리
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

    // [Added] 종료 UI 공통 처리
    void ShowEndScreen(EEndReason Reason);
    void ShowWidgetAndFocus(TSubclassOf<UUserWidget> WidgetClass);

public:
    UFUNCTION()
    void OnEnemyKilled(AActor* DestroyedActor);

    // [Added] HUD/블루프린트용 훅
    UPROPERTY(BlueprintAssignable, Category = "UI")
    FOnGameEnded OnGameEnded;

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

    /** 점수 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
    int32 Score;

    /** 웨이브 데이터 에셋 (BP에서 설정) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave|Settings")
    UWaveDataAsset* WaveData;

    /** 월드에 배치된 스폰 매니저 참조 (BeginPlay에서 자동 탐색) */
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Wave|Runtime")
    ASpartaSpawnManager* SpawnManagerRef;

    /** 대기 시간용 타이머 핸들 */
    FTimerHandle InterWaveTickHandle;

    // [Added] 종료 화면 위젯 클래스 (에디터에서 지정)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameClearedWidgetClass;

    // [Added] 종료 UI 중복 표시 방지
    bool bEndScreenShown = false;
};
