#include "Weapons/Actors/Guns/Pistol.h"

APistol::APistol()
{
	FireType = ERangeFireType::SingleShot;
	WeaponName = "Pistol";
	Power = 5;
	RateOfFire = 0.3f;

	LeverType = ERangeLeverType::Single;
	FireSpeed = 1;
	MaxBulletAmount = 12;
	CurBulletAmount = MaxBulletAmount;
	bIsFire = true;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
    bIsVisible = true;
    SetActorHiddenInGame(false);
    SetActorTickEnabled(false);

    SetFireState();
    Attack(this);
}

void APistol::StartFire()
{
    Super::StartFire();
    Attack(this);
}

void APistol::StopFire()
{
    Super::StopFire();
}
