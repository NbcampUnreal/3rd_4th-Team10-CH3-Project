#pragma once

#include "CoreMinimal.h"
#include "Items/Actors/ItemBase.h"
#include "BuffItemBase.generated.h"

UCLASS()
class TEAM10_PROJECT_API ABuffItemBase : public AItemBase
{
	GENERATED_BODY()
	
protected:
    ABuffItemBase();

    virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) override;

    virtual void InteractiveItem(AActor* Activate) override;
    virtual void GetItem(AActor* Activate);
    virtual void ImmediatelyItem(AActor* Activate);

    EItemUsableType UsableType;
};
