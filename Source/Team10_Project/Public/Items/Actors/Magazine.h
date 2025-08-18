#pragma once

#include "CoreMinimal.h"
#include "Items/Actors/ItemBase.h"
#include "Magazine.generated.h"

UCLASS()
class TEAM10_PROJECT_API AMagazine : public AItemBase
{
	GENERATED_BODY()
	
private:
    int32 MuzzleAmount;

public:
    AMagazine();

    virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) override;
    virtual void InteractiveItem(AActor*) override;
};
