#include "SpartaGameMode.h"
#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASpartaGameMode::ASpartaGameMode()
{
    PrimaryActorTick.bCanEverTick = false;

    // 웨이브 설정 초기값
    MaxWave = 3;
    BaseEnemyCount = 3;
    WaveEnemyMultiplier = 2;
    InterWaveDelay = 5.0f;

    // 런타임 상태 초기값
    WaveCounter = 0;
    EnemiesToSpawn = 0;
    EnemiesRemaining = 0;
    bWaitingForNextWave = false;
    RemainingInterWaveTime = 0;
}

void ASpartaGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("Game Start"));

    // 첫 웨이브 시작
    StartWave();
}

void ASpartaGameMode::StartWave()
{
    // 웨이브 번호 증가
    WaveCounter++;

    // 적 수 계산
    EnemiesToSpawn = BaseEnemyCount + WaveEnemyMultiplier * WaveCounter;
    EnemiesRemaining = EnemiesToSpawn;

    // GameState에 데이터 전달
    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetDisplayWave(WaveCounter);
        GS->SetDisplayEnemiesRemaining(EnemiesRemaining);
    }

    // TODO: 적 스폰 호출
    // if (SpawnManager) { SpawnManager->SpawnEnemies(EnemiesToSpawn); }

    UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작! 적 수: %d"), WaveCounter, EnemiesToSpawn);
}

void ASpartaGameMode::EndWave()
{
    // 최대 웨이브 도달 시 게임 클리어
    if (WaveCounter >= MaxWave)
    {
        OnGameCleared();
        return;
    }

    // 준비 시간 시작
    StartInterWaveDelay();
}

void ASpartaGameMode::StartInterWaveDelay()
{
    bWaitingForNextWave = true;
    RemainingInterWaveTime = FMath::RoundToInt(InterWaveDelay);

    // GameState에 UI 표시용 남은 시간 전달
    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetInterWaveCountdown(RemainingInterWaveTime);
    }

    // 1초마다 Tick 호출
    GetWorldTimerManager().SetTimer(
        InterWaveTickHandle,
        this,
        &ASpartaGameMode::TickInterWaveDelay,
        1.0f,
        true
    );
}

void ASpartaGameMode::TickInterWaveDelay()
{
    // 남은 시간 감소
    RemainingInterWaveTime--;

    // GameState에 UI용 값 갱신
    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->SetInterWaveCountdown(RemainingInterWaveTime);
    }

    // 준비 시간 끝나면 웨이브 시작
    if (RemainingInterWaveTime <= 0)
    {
        GetWorldTimerManager().ClearTimer(InterWaveTickHandle);
        bWaitingForNextWave = false;
        StartWave();
    }
}

void ASpartaGameMode::OnEnemyKilled()
{
    // 남은 적 수 감소
    EnemiesRemaining--;

    // GameState에 점수 및 적 수 갱신
    if (ASpartaGameState* GS = GetGameState<ASpartaGameState>())
    {
        GS->AddScore(10); // 적 처치 시 10점 추가
        GS->DecreaseRemainingEnemies();
    }

    // 모든 적 처치 시 웨이브 종료
    if (EnemiesRemaining <= 0)
    {
        EndWave();
    }
}

void ASpartaGameMode::OnPlayerDied()
{
    UE_LOG(LogTemp, Warning, TEXT("플레이어 사망!"));
    OnGameOver();
}

void ASpartaGameMode::OnGameCleared()
{
    UE_LOG(LogTemp, Warning, TEXT("게임 클리어!"));
}

void ASpartaGameMode::OnGameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("게임 오버"));
}
