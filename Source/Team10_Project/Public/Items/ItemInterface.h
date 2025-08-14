#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Items/ItemTypes.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM10_PROJECT_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
        int32 OtherBodyIndex,
		bool bFromSweep, 
        const FHitResult& SweepResult) = 0;
    UFUNCTION()
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) = 0;
    virtual void InteractiveItem(AActor* Activate) = 0;
    virtual void VisibleItem() = 0;
    virtual void InVisibleItem() = 0;
    virtual EItemType GetItemType() = 0;
    virtual bool GetItemOverlapState() = 0;
};
