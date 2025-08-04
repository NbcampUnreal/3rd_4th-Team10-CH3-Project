#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/ObjectPoolManager.h"
#include "ObjectPoolComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM10_PROJECT_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolComponent();

protected:
	virtual void BeginPlay() override;

private:
	AObjectPoolManager* Pool;

public:

	template <typename T>
	T* GetObject()
	{
		UClass* ObjectClass = T::StaticClass();
		for (AActor* Object : Pool->ObjectPool)
		{
			if (!Cast<T>(Object)->GetIsActive())
			{
				Cast<T>(Object)->ActiveObject();
				return Cast<T>(Object);
			}
		}

		T* NewObject = GetWorld()->SpawnActor<T>(ObjectClass);
		Pool->ObjectPool.Add(NewObject);
		return NewObject;
	}
	template <typename T>
	void ReturnObject(T* Object)
	{
		Object->DeActivate();
	}
};
