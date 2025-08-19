#include "SpartaGameMode.h"
#include "SpartaGameState.h"
#include "SpartaSpawnManager.h"
#include "WaveData.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"                 // [Added]
#include "GameFramework/PlayerController.h"       // [Added]

ASpartaGameMode::ASpartaGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    WaveCounter = 0;
    UE_LOG(LogTemp, Error, TEXT("SpartaGameMode C++ CONSTRUCTOR IS RUNNING!"));
    bWaitingForNextWave = false;
    EnemiesRemaining = 0;
    InterWaveCountdown = 0.0f;
    Score = 0;
    bEndScreenShown = false; // [Added]
}

void ASpartaGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Error, TEXT("SpartaGameMode C++ BeginPlay IS RUNNING!"));

    SpawnManagerRef = Cast<ASpartaSpawnManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ASpartaSpawnManager::StaticClass())
    );

    StartWave();
}

void ASpartaGameMode::StartWave()
{
    WaveCounter++;
    const int32 WaveIndex = WaveCounter - 1;

    if (!WaveData || !WaveData->Waves.IsValidIndex(WaveIndex))
    {
        OnGameCleared();
        return;
    }

    const FWaveData& CurrentWave = WaveData->Waves[WaveIndex];

    EnemiesRemaining = 0;
    for (const FEnemySpawnInfo& Info : CurrentWave.EnemiesToSpawn)
    {
        EnemiesRemaining += Info.SpawnCount;
    }

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayWave(WaveCounter);
        GS->SetDisplayEnemiesRemaining(EnemiesRemaining);
        GS->SetDisplayScore(Score);
    }

    if (SpawnManagerRef)
    {
        // 태그 기반 스폰 포인트 탐색
        FName DesiredTag = CurrentWave.SpawnPointTag;
        AActor* FoundSpawnPoint = nullptr;

        if (!DesiredTag.IsNone())
        {
            TArray<AActor*> ActorsWithTag;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), DesiredTag, ActorsWithTag);
            if (ActorsWithTag.Num() > 0)
            {
                FoundSpawnPoint = ActorsWithTag[0];
            }
        }

        if (FoundSpawnPoint)
        {
            UE_LOG(LogTemp, Log, TEXT("Wave %d starting at spawn point with tag '%s'"),
                WaveCounter, *DesiredTag.ToString());
            SpawnManagerRef->StartSpawning(CurrentWave, FoundSpawnPoint);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find Actor with tag '%s' for Wave %d!"),
                *DesiredTag.ToString(), WaveCounter);
        }
    }
}

void ASpartaGameMode::OnEnemyKilled(AActor* /*DestroyedActor*/)
{
    EnemiesRemaining--;
    Score += 10; // 기본 가점

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayEnemiesRemaining(EnemiesRemaining);
        GS->SetDisplayScore(Score);
    }

    if (EnemiesRemaining <= 0)
    {
        EndWave();
    }
}

void ASpartaGameMode::EndWave()
{
    const int32 WaveIndex = WaveCounter - 1;

    if (!WaveData || !WaveData->Waves.IsValidIndex(WaveIndex))
        return;

    const float Delay = WaveData->Waves[WaveIndex].TimeUntilNextWave;
    StartInterWaveDelay(Delay);
}

void ASpartaGameMode::StartInterWaveDelay(float InDelay)
{
    bWaitingForNextWave = true;
    InterWaveCountdown = InDelay;

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetWaitingWave(bWaitingForNextWave);
    }

    TickInterWaveDelay();

    GetWorldTimerManager().SetTimer(
        InterWaveTickHandle, this, &ASpartaGameMode::TickInterWaveDelay, 1.0f, true);
}

void ASpartaGameMode::TickInterWaveDelay()
{
    InterWaveCountdown--;

    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayCountdown(InterWaveCountdown);
    }

    if (InterWaveCountdown <= 0)
    {
        GetWorldTimerManager().ClearTimer(InterWaveTickHandle);
        bWaitingForNextWave = false;
        if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
        {
            GS->SetWaitingWave(bWaitingForNextWave);
        }
        StartWave();
    }
}

void ASpartaGameMode::OnGameCleared()
{
    if (bEndScreenShown) return;        // [Added]
    UE_LOG(LogTemp, Warning, TEXT("Game Cleared!"));

    OnGameEnded.Broadcast(EEndReason::Cleared);   // [Added] HUD 훅
    ShowEndScreen(EEndReason::Cleared);           // [Added] 기본 위젯 표시
}

void ASpartaGameMode::OnPlayerDied()
{
    UE_LOG(LogTemp, Warning, TEXT("Player Died - Game Over"));
    OnGameOver();
}

void ASpartaGameMode::OnGameOver()
{
    if (bEndScreenShown) return;        // [Added]
    UE_LOG(LogTemp, Warning, TEXT("Game Over"));

    OnGameEnded.Broadcast(EEndReason::GameOver);  // [Added]
    ShowEndScreen(EEndReason::GameOver);          // [Added]
}

// [Added] 종료 UI 공통 처리
void ASpartaGameMode::ShowEndScreen(EEndReason Reason)
{
    if (bEndScreenShown) return;

    TSubclassOf<UUserWidget> WidgetClassToShow = nullptr;

    switch (Reason)
    {
    case EEndReason::Cleared:
        WidgetClassToShow = GameClearedWidgetClass;
        break;
    case EEndReason::GameOver:
        WidgetClassToShow = GameOverWidgetClass;
        break;
    default:
        break;
    }

    if (WidgetClassToShow)
    {
        ShowWidgetAndFocus(WidgetClassToShow);
        bEndScreenShown = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("End screen widget class not set in GameMode."));
    }

    // 필요 시 일시정지 적용
    // APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    // if (PC) { SetPause(PC); }
}

// [Added] UI 전용 입력 모드/커서 처리
void ASpartaGameMode::ShowWidgetAndFocus(TSubclassOf<UUserWidget> WidgetClass)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !WidgetClass) return;

    UUserWidget* Widget = CreateWidget<UUserWidget>(PC, WidgetClass);
    if (!Widget) return;

    Widget->AddToViewport(100);
    PC->bShowMouseCursor = true;

    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(Widget->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
}
