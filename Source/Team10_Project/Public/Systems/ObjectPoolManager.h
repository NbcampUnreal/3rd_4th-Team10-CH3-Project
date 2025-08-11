#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Actors/Bullet.h"
#include <Kismet/GameplayStatics.h>
#include "ObjectPoolManager.generated.h"

UCLASS()
class TEAM10_PROJECT_API AObjectPoolManager : public AActor
{
	GENERATED_BODY()

public:	
	AObjectPoolManager();

protected:
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "PoolData")
	int32 PoolSize;

public:
	UPROPERTY(EditAnywhere, Category = "PoolData")
	TArray<TSubclassOf <AActor>> PoolObjectData;
	UPROPERTY(EditAnywhere, Category = "PoolData")
    TArray<AActor*> ObjectPool;

	void InitPool()
	{
		for (TSubclassOf<AActor> ActorToSpawn : PoolObjectData)
		{
			for (int32 i = 0; i < PoolSize; ++i)
			{
				AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(ActorToSpawn,
					FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

                UGameplayStatics::FinishSpawningActor(NewActor, FTransform::Identity);
				if (NewActor->GetClass()->ImplementsInterface(UPoolObjectInterface::StaticClass()))
				{
					IPoolObjectInterface::Execute_SetActive(NewActor, true);
					IPoolObjectInterface::Execute_DeActiveObject(NewActor);
				}

				ObjectPool.Add(NewActor);
			}
		}
	}

	template <typename T>
	T* GetObject()
	{
		UClass* ObjectClass = T::StaticClass();

		for (AActor* Object : ObjectPool)
		{
			if (Object && Object->GetClass()->IsChildOf(ObjectClass))
			{
				if (Object->GetClass()->ImplementsInterface(UPoolObjectInterface::StaticClass()))
				{
					if (!IPoolObjectInterface::Execute_GetIsActive(Object))
					{
						IPoolObjectInterface::Execute_ActiveObject(Object);
						return Cast<T>(Object);
					}
				}
			}
		}

        T* NewObject = GetWorld()->SpawnActorDeferred<T>(ObjectClass,
            FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        UGameplayStatics::FinishSpawningActor(NewObject, FTransform::Identity);
        if (NewObject->GetClass()->ImplementsInterface(UPoolObjectInterface::StaticClass()))
        {
            IPoolObjectInterface::Execute_ActiveObject(NewObject);
        }
        ObjectPool.Add(NewObject);

        return NewObject;
		
	}

	template <typename T>
	void ReturnObject(T* Object)
	{
		if (Object && Object->GetClass()->ImplementsInterface(UPoolObjectInterface::StaticClass()))
		{
			IPoolObjectInterface::Execute_DeActiveObject(Object);
		}
	}
};
