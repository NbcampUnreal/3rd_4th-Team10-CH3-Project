#include "SpartaGameState.h"

void ASpartaGameState::SetDisplayWave(int32 NewWave)
{
    DisplayWave = NewWave;
}

void ASpartaGameState::SetDisplayEnemiesRemaining(int32 NewCount)
{
    DisplayEnemiesRemaining = NewCount;
}

void ASpartaGameState::SetDisplayCountdown(float TimeLeft)
{
    DisplayCountdown = TimeLeft;
}

void ASpartaGameState::SetWaitingWave(bool bWaiting)
{
    bWaitingWave = bWaiting;
}
