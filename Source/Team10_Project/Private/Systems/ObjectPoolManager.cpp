#include "Systems/ObjectPoolManager.h"
#include "Systems/PoolObjectDataAsset.h"

UObjectPoolManager::UObjectPoolManager()
{
	PoolSize = 10;
	static ConstructorHelpers::FObjectFinder<UPoolObjectDataAsset> PoolAsset(TEXT("/Game/DataAsset/PoolingObjectDataAsset.PoolingObjectDataAsset"));
	if (PoolAsset.Succeeded())
	{
		PoolObjectData = PoolAsset.Object->PoolClasses;
	}
}

void UObjectPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	InitPool();
}