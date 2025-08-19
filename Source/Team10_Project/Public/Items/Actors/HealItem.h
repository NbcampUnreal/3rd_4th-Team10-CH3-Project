#pragma once

#include "CoreMinimal.h"
#include "Items/Actors/BuffItemBase.h"
#include "HealItem.generated.h"

UCLASS()
class TEAM10_PROJECT_API AHealItem : public ABuffItemBase
{
	GENERATED_BODY()
	
    AHealItem();

    int32 HealAmount;

    virtual void ImmediatelyItem(AActor* Activate) override;
    virtual void InteractiveItem(AActor*) override;
};