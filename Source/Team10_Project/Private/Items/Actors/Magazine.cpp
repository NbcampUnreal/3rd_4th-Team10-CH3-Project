#include "Items/Actors/Magazine.h"
#include "MyCharacter.h"

AMagazine::AMagazine()
{
    ItemType = EItemType::Weapon;
    ItemName = "Magazine";

    MuzzleAmount = 30;
}

void AMagazine::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp, 
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, 
    bool bFromSweep, 
    const FHitResult& SweepResult)
{
    Super::OnItemOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    /*
    AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
    if (Character)
    {
        Character->SetAmmoAmount(MuzzleAmount);
        DestoryItem();
    }
    */
}

void AMagazine::InteractiveItem(AActor* Actor)
{
    AMyCharacter* Character = Cast<AMyCharacter>(Actor);
    if (Character)
    {
        Character->SetAmmoAmount(MuzzleAmount);
        DestoryItem();
    }
}