#include "Items/Actors/HealItem.h"
#include "Items/ItemInterface.h"
#include "MyCharacter.h"
#include "AttributeComponent.h"

AHealItem::AHealItem()
{
    UsableType = EItemUsableType::Immediately;
    ItemName = "HealItem";

    HealAmount = 50.0f;
}

void AHealItem::ImmediatelyItem(AActor* Activate)
{
    Super::ImmediatelyItem(Activate);

    AMyCharacter* Character = Cast<AMyCharacter>(Activate);
    Character->GetAttributeComponent()->ModityHealth(HealAmount);

    InVisibleItem();
}