#include "Items/Actors/HealItem.h"
#include "Items/ItemInterface.h"

AHealItem::AHealItem()
{
    UsableType = EItemUsableType::Immediately;
    ItemName = "HealItem";

    HealAmount = 50.0f;
}

void AHealItem::ImmediatelyItem(AActor* Activate)
{
    Super::ImmediatelyItem(Activate);
}