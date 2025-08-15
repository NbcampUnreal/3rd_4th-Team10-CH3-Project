#include "Weapons/Actors/Guns/Pistol.h"

APistol::APistol()
{
    RangeType = ERangeType::Pistol;
	FireType = ERangeFireType::SingleShot;
	WeaponName = "Pistol";
	Power = 3;
	RateOfFire = 0.3f;

	FireSpeed = 1.0f;
	MaxBulletAmount = 12;
    LoadAmmoAmount = MaxBulletAmount;
    ConsumeAmmoAmount = 1;
	bIsFire = true;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
    bIsVisible = true;
    SetActorHiddenInGame(false);
    SetActorTickEnabled(false);

    SetFireState();
}

void APistol::StartFire()
{
    Super::StartFire();
    Attack(this);
}
