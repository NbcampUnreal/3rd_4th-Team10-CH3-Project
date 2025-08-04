#include "Weapons/Actors/Bullet.h"

ABullet::ABullet()
{
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	WeaponStaticMesh->SetupAttachment(Scene);

	WeaponName = "Bullet";
	ProjectileSpeed = 850.0f;
	ProjectileRange = 800.0f;
	Power = 10;

	Height = 1.0f;
	Width = 1.0f;
	Vertical = 1.0f;
}
