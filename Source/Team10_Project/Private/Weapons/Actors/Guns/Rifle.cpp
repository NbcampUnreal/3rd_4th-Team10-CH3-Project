#include "Weapons/Actors/Guns/Rifle.h"

ARifle::ARifle()
{
    RangeType = ERangeType::Rifle;
    LeverType = ERangeLeverType::Repeatedly;
    WeaponName = "Rifle";
    Power = 5;
    RateOfFire = 0.05f;

    FireSpeed = 1.0f;
    MaxBulletAmount = 30;
    LoadAmmoAmount = MaxBulletAmount;
    ConsumeAmmoAmount = 1;
    bIsFire = true;
}

void ARifle::BeginPlay()
{
    Super::BeginPlay();
}

void ARifle::StartFire()
{
    Super::StartFire();

    RemainingFireCount = FireCount;

    if (FireType == ERangeFireType::SingleShot)
    {
        Attack(this);
    }
    else
    {
        Attack(this);

        FTimerDelegate Delegate;
        Delegate.BindUObject(this, &AWeaponBase::Attack, Cast<AActor>(this));

        GetWorld()->GetTimerManager().SetTimer(
            FireCountHandle,
            Delegate,
            RateOfFire,
            true
        );
    }
}
