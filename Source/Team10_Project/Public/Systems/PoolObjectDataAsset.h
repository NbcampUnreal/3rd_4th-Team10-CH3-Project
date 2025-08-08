#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PoolObjectDataAsset.generated.h"

UCLASS()
class TEAM10_PROJECT_API UPoolObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> PoolClasses;
};
