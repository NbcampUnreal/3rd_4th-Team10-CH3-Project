#include "Systems/ObjectPoolManager.h"

AObjectPoolManager::AObjectPoolManager()
{
	PrimaryActorTick.bCanEverTick = true;

	PoolSize = 10;
}

void AObjectPoolManager::BeginPlay()
{
	Super::BeginPlay();
	InitPool();
}