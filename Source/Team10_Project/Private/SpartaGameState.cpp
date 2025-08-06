#include "SpartaGameState.h"

void ASpartaGameState::AddScore(int32 Amount)
{
    DisplayScore += Amount;
}

void ASpartaGameState::DecreaseRemainingEnemies()
{
    DisplayEnemiesRemaining = FMath::Max(DisplayEnemiesRemaining - 1, 0);
}

void ASpartaGameState::SetDisplayWave(int32 Wave)
{
    DisplayWave = Wave;
}

void ASpartaGameState::SetDisplayEnemiesRemaining(int32 Count)
{
    DisplayEnemiesRemaining = Count;
}

void ASpartaGameState::SetInterWaveCountdown(int32 Time)
{
    InterWaveCountdown = Time;
}
