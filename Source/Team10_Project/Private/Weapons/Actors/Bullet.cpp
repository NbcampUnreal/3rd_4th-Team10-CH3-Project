#include "Weapons/Actors/Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	WeaponName = "Bullet";
	ProjectileSpeed = 50.0f;
	ProjectileRange = 8000.0f;
	Power = 10;

    BoxExtent = ProjectileCollision->GetScaledBoxExtent();
	CollisionSize = BoxExtent * 2;
	Height = CollisionSize.Z;
	Width = CollisionSize.X;
	Vertical = CollisionSize.Y;

	LifeTime = 100.0f;
}

void ABullet::ProjectileMovement()
{
	Super::ProjectileMovement();
}