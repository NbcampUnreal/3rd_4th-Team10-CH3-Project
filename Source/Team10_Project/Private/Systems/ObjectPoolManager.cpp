#include "Systems/ObjectPoolManager.h"
#include "Systems/PoolObjectDataAsset.h"

AObjectPoolManager::AObjectPoolManager()
{
	PoolSize = 10;
	static ConstructorHelpers::FObjectFinder<UPoolObjectDataAsset> PoolAsset(TEXT("/Game/DataAsset/PoolingObjectDataAsset.PoolingObjectDataAsset"));
	if (PoolAsset.Succeeded())
	{
		PoolObjectData = PoolAsset.Object->PoolClasses;
	}
}
void AObjectPoolManager::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    InitPool();
}
void AObjectPoolManager::BeginPlay()
{
    Super::BeginPlay();
}