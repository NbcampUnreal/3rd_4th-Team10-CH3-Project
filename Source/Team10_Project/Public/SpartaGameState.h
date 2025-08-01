#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpartaGameState.generated.h"

UCLASS()
class TEAM10_PROJECT_API ASpartaGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    // 점수 증가
    void AddScore(int32 Amount);

    // 적 수 감소
    void DecreaseRemainingEnemies();

    // UI 연동용
    void SetDisplayWave(int32 Wave);
    void SetDisplayEnemiesRemaining(int32 Count);

    // Getter (블루프린트에서 사용 가능)
    UFUNCTION(BlueprintCallable)
    int32 GetDisplayWave() const { return DisplayWave; }

    UFUNCTION(BlueprintCallable)
    int32 GetDisplayEnemiesRemaining() const { return DisplayEnemiesRemaining; }

    UFUNCTION(BlueprintCallable)
    int32 GetDisplayScore() const { return DisplayScore; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 DisplayWave;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 DisplayEnemiesRemaining;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Display")
    int32 DisplayScore;
};
