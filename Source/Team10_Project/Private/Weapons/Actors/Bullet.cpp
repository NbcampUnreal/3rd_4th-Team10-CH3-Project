#include "Weapons/Actors/Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	WeaponName = "Bullet";
	ProjectileSpeed = 200.0f;
	ProjectileRange = 8000.0f;
	Power = 10;
    bIsVisible = true;
    Only = true;
}

void ABullet::BeginPlay()
{
    Super::BeginPlay();
    InVisibleItem();
    SetActorTickEnabled(true);
}

void ABullet::ProjectileMovement()
{
	Super::ProjectileMovement();
}