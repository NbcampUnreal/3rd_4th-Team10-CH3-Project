#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveData.generated.h"

/** 스폰할 적의 정보 */
USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
    GENERATED_BODY()

    /** 스폰할 적의 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Data")
    TSubclassOf<APawn> EnemyClass;

    /** 이 클래스의 적을 몇 마리 스폰할지 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Data")
    int32 SpawnCount = 1;
};

/** 하나의 웨이브에 대한 전체 구성 정보 */
USTRUCT(BlueprintType)
struct FWaveData
{
    GENERATED_BODY()

    /** 이 웨이브에 등장할 적 리스트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Data")
    TArray<FEnemySpawnInfo> EnemiesToSpawn;

    /** 이 웨이브 후 다음 웨이브까지의 대기 시간 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Data")
    float TimeUntilNextWave = 5.0f;
};

/** 웨이브 전체를 담는 데이터 애셋 */
UCLASS(Blueprintable, BlueprintType)
class TEAM10_PROJECT_API UWaveDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    /** 모든 웨이브 정보 배열 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Data")
    TArray<FWaveData> Waves;
};
