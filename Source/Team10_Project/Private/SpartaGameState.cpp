#include "SpartaGameState.h"

ASpartaGameState::ASpartaGameState()
{
    DisplayWave = 0;
    DisplayEnemiesRemaining = 0;
    DisplayCountdown = 0.0f;
    bWaitingWave = false;
}

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
