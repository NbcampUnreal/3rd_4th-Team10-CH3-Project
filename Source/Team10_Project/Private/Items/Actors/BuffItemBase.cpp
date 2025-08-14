#include "Items/Actors/BuffItemBase.h"


ABuffItemBase::ABuffItemBase()
{
    ItemType = EItemType::Buff;
}

void ABuffItemBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    Super::OnItemOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (UsableType == EItemUsableType::Immediately)
    {
        ImmediatelyItem(OtherActor);
    }
    else if (UsableType == EItemUsableType::Acquire)
    {

    }
}

void ABuffItemBase::InteractiveItem(AActor* Activate)
{

}

void ABuffItemBase::GetItem(AActor* Activate)
{
}

void ABuffItemBase::ImmediatelyItem(AActor* Activate)
{
}
