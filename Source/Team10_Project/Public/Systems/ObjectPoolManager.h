#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Actors/Bullet.h"
#include "Systems/SingletonSubsystem.h"
#include "ObjectPoolManager.generated.h"

UCLASS()
class TEAM10_PROJECT_API UObjectPoolManager : public USingletonSubsystem
{
	GENERATED_BODY()

public:	
	UObjectPoolManager();

protected:
	void Initialize(FSubsystemCollectionBase& Collection) override;
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
				Cast<IPoolObjectInterface>(NewActor)->bIsActive = true;
				if (NewActor->GetClass()->ImplementsInterface(UPoolObjectInterface::StaticClass()))
				{
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
			if (!Cast<T>(Object)->GetIsActive())
			{
				Cast<T>(Object)->ActiveObject();
				return Cast<T>(Object);
			}
		}

		T* NewObject = GetWorld()->SpawnActor<T>(ObjectClass);
		ObjectPool.Add(NewObject);
		return NewObject;
	}
	template <typename T>
	void ReturnObject(T* Object)
	{
		Object->DeActiveObject();
	}
};
