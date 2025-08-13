#include "Weapons/Actors/Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	WeaponName = "Bullet";
	ProjectileSpeed = 50.0f;
	ProjectileRange = 8000.0f;
	Power = 10;
}

void ABullet::ProjectileMovement()
{
	Super::ProjectileMovement();
}