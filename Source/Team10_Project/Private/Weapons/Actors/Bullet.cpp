#include "Weapons/Actors/Bullet.h"

ABullet::ABullet()
{
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	WeaponStaticMesh->SetupAttachment(Scene);
	ProjectileSpeed = 850.0f;
	ProjectileRange = 800.0f;
	Power = 10;
}
