#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Items/Actors/ItemBase.h"
#include "ItemSpawnVolume.generated.h"

// --- 데이터 테이블의 행 구조체 ---
// 아이템 종류와 스폰 가중치를 정의합니다.
USTRUCT(BlueprintType)
struct FItemSpawnInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 스폰할 아이템의 클래스 (AItemBase를 상속받은 클래스)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AItemBase> ItemClass;

    // 스폰 가중치. 높을수록 더 잘 나옵니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.1"))
    float SpawnWeight = 1.0f;
};

// --- 아이템 스폰 볼륨 액터 ---
UCLASS()
class TEAM10_PROJECT_API AItemSpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    AItemSpawnVolume();

    // 아이템 스폰을 시작하는 함수
    void SpawnItems();

protected:
    // 스폰의 경계가 되는 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* SpawnVolume;

    // 스폰할 아이템 목록이 들어있는 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    class UDataTable* ItemDataTable;

    // 한 웨이브에 스폰할 아이템의 총 개수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    int32 TotalSpawnCount = 10;
};
