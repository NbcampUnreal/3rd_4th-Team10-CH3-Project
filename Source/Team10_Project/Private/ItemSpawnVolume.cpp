#include "Public/ItemSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AItemSpawnVolume::AItemSpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
    RootComponent = SpawnVolume;
}

void AItemSpawnVolume::SpawnItems()
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("AItemSpawnVolume: ItemDataTable is not set. Cannot spawn items."));
        return;
    }

    // 데이터 테이블의 모든 행 이름을 가져옵니다.
    const TArray<FName> RowNames = ItemDataTable->GetRowNames();
    if (RowNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("AItemSpawnVolume: ItemDataTable is empty."));
        return;
    }

    // 전체 가중치 계산
    float TotalWeight = 0.0f;
    for (const FName& RowName : RowNames)
    {
        FItemSpawnInfo* Row = ItemDataTable->FindRow<FItemSpawnInfo>(RowName, TEXT(""));
        if (Row && Row->ItemClass)
        {
            TotalWeight += Row->SpawnWeight;
        }
    }

    // TotalSpawnCount 만큼 스폰 반복
    for (int32 i = 0; i < TotalSpawnCount; ++i)
    {
        // 랜덤 값 선택
        float Pick = FMath::FRandRange(0.f, TotalWeight);
        float CurrentWeight = 0.f;

        // 가중치 기반 아이템 선택
        for (const FName& RowName : RowNames)
        {
            FItemSpawnInfo* Row = ItemDataTable->FindRow<FItemSpawnInfo>(RowName, TEXT(""));
            if (Row && Row->ItemClass)
            {
                CurrentWeight += Row->SpawnWeight;
                if (Pick <= CurrentWeight)
                {
                    // 스폰 위치 랜덤 결정
                    const FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
                    const FVector SpawnExtent = SpawnVolume->Bounds.BoxExtent;
                    const FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);

                    // 아이템 스폰
                    GetWorld()->SpawnActor<AItemBase>(Row->ItemClass, SpawnLocation, FRotator::ZeroRotator);

                    break; // 다음 아이템 스폰으로
                }
            }
        }
    }
}
