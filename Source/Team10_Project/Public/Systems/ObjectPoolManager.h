#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Actors/Bullet.h"
#include "ObjectPoolManager.generated.h"

UCLASS()
class TEAM10_PROJECT_API AObjectPoolManager : public AActor
{
	GENERATED_BODY()

public:	
	AObjectPoolManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "PoolData")
	int32 PoolSize;
	UPROPERTY(EditAnywhere, Category = "PoolData")
	TArray<TSubclassOf<AActor>> PoolObjectData;
public:
	UPROPERTY(EditAnywhere, Category = "PoolData")
	TArray<AActor*> ObjectPool;

	void InitPool()
	{
		for (int i = 0; i < PoolObjectData.Num(); i++)
		{
			for (int32 j = 0; j < PoolSize; ++j)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AActor* NewActor = GetWorld()->SpawnActor<AActor>(PoolObjectData[i], 
					FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
				if (NewActor->GetClass()->ImplementsInterface(UPoolObjectInterface::StaticClass()))
				{
					IPoolObjectInterface::Execute_DeActiveObject(NewActor);
				}
				ObjectPool.Add(NewActor);
			}
		}
	}
};
