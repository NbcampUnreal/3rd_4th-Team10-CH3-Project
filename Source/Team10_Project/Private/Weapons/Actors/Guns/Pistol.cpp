#include "Weapons/Actors/Guns/Pistol.h"

APistol::APistol()
{
    RangeType = ERangeType::Pistol;
    LeverType = ERangeLeverType::Single;
	WeaponName = "Pistol";
	Power = 3;
	RateOfFire = 0.1f;

	FireSpeed = 1.0f;
	MaxBulletAmount = 12;
    LoadAmmoAmount = MaxBulletAmount;
    ConsumeAmmoAmount = 1;
	bIsFire = true;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
}

void APistol::StartFire()
{
    Super::StartFire();

    RemainingFireCount = FireCount;
    FTimerDelegate Delegate;
    Delegate.BindUObject(this, &AWeaponBase::Attack, Cast<AActor>(this));

    GetWorld()->GetTimerManager().SetTimer(
        FireCountHandle,
        Delegate,
        RateOfFire,
        false
    );
}
