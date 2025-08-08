#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolObjectInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPoolObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM10_PROJECT_API IPoolObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PoolObject")
	void SetActive(bool Active);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PoolObject")
	bool GetIsActive();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PoolObject")
	void ActiveObject();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PoolObject")
	void DeActiveObject();
};
