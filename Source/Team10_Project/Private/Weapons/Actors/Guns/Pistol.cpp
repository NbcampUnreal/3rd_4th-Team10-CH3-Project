#include "Weapons/Actors/Guns/Pistol.h"

APistol::APistol()
{
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(Scene);

	ProjectilePoint = WeaponSkeletalMesh->GetSocketLocation(TEXT("MuzzleSocket"));

	FireType = ERangeFireType::SingleShot;
	WeaponName = "Pistol";
	Power = 5;
	RateOfFire = 0.3f;

	LeverType = ERangeLeverType::Single;
	FireSpeed = 1;
	MaxBulletAmount = 10;
	CurBulletAmount = MaxBulletAmount;
	bIsFire = true;
}