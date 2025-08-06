#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpartaGameState.generated.h"

/**
 * ASpartaGameState
 * - UI와 연동되는 게임 상태(점수, 웨이브, 남은 적 수 등)를 관리하는 클래스
 * - BlueprintReadOnly 속성으로 UMG에서 읽기 가능
 */
UCLASS()
class TEAM10_PROJECT_API ASpartaGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    /** 점수 추가 */
    void AddScore(int32 Amount);

    /** 남은 적 수 감소 */
    void DecreaseRemainingEnemies();

    /** 웨이브 수 설정 */
    void SetDisplayWave(int32 Wave);

    /** 남은 적 수 설정 */
    void SetDisplayEnemiesRemaining(int32 Count);

    /** 웨이브 대기시간 설정 */
    void SetInterWaveCountdown(int32 Time);

    //=========================
    //  Getter for UI
    //=========================

    /** 현재 웨이브 반환 (UI용) */
    UFUNCTION(BlueprintCallable)
    int32 GetDisplayWave() const { return DisplayWave; }

    /** 남은 적 수 반환 (UI용) */
    UFUNCTION(BlueprintCallable)
    int32 GetDisplayEnemiesRemaining() const { return DisplayEnemiesRemaining; }

    /** 현재 점수 반환 (UI용) */
    UFUNCTION(BlueprintCallable)
    int32 GetDisplayScore() const { return DisplayScore; }

    /** 웨이브 대기 시간 반환 (UI용) */
    UFUNCTION(BlueprintCallable)
    int32 GetInterWaveCountdown() const { return InterWaveCountdown; }

protected:
    //=========================
    //  UI에 보여줄 상태 변수
    //=========================

    /** 현재 웨이브 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 DisplayWave;

    /** 남은 적 수 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 DisplayEnemiesRemaining;

    /** 점수 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 DisplayScore;

    /** 다음 웨이브까지 남은 대기 시간 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 InterWaveCountdown;
};
