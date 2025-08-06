#include "Systems/ObjectPoolManager.h"

UObjectPoolManager::UObjectPoolManager()
{
	PoolSize = 10;
}

void UObjectPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitPool();
}