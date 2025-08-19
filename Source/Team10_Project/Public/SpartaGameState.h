// ===============================
// SpartaGameState.h
// 게임의 현재 상태를 UI에 전달하는 GameState 클래스
// ===============================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpartaGameState.generated.h"

UCLASS()
class TEAM10_PROJECT_API ASpartaGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ASpartaGameState();

    // 현재 웨이브 번호 설정 (UI에서 표시용)
    void SetDisplayWave(int32 NewWave);

    // 현재 남은 적 수 설정
    void SetDisplayEnemiesRemaining(int32 NewCount);

    // 인터웨이브 카운트다운 설정
    void SetDisplayCountdown(float TimeLeft);

    // 웨이브 대기시간 bool 설정
    void SetWaitingWave(bool bWaiting);

    // 스코어 설정
    void SetDisplayScore(int32 NewScore);

    // Getter 함수들 (UI 바인딩용으로 사용 가능)
    UFUNCTION(BlueprintPure, Category = "Wave|Display")
    int32 GetDisplayWave() const { return DisplayWave; }
    UFUNCTION(BlueprintPure, Category = "Wave|Display")
    int32 GetDisplayEnemiesRemaining() const { return DisplayEnemiesRemaining; }
    UFUNCTION(BlueprintPure, Category = "Wave|Display")
    float GetDisplayCountdown() const { return DisplayCountdown; }
    UFUNCTION(BlueprintPure, Category = "Wave|Display")
    bool IsWaitingForNextWave() const { return bWaitingWave; }
    UFUNCTION(BlueprintPure, Category = "Game")
    int32 GetDisplayScore() const { return DisplayScore; }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Wave|Display")
    int32 DisplayWave;

    UPROPERTY(BlueprintReadOnly, Category = "Wave|Display")
    int32 DisplayEnemiesRemaining;

    UPROPERTY(BlueprintReadOnly, Category = "Wave|Display")
    float DisplayCountdown;

    UPROPERTY(BlueprintReadOnly, Category = "Wave|Display")
    bool bWaitingWave;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    int32 DisplayScore;
};
