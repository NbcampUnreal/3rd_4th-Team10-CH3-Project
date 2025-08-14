#include "Weapons/Actors/Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	WeaponName = "Bullet";
	ProjectileSpeed = 100.0f;
	ProjectileRange = 8000.0f;
	Power = 10;
    bIsVisible = true;
}

void ABullet::BeginPlay()
{
    Super::BeginPlay();
    GetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCollision->Deactivate();
    SetActorTickEnabled(true);
}

void ABullet::ProjectileMovement()
{
	Super::ProjectileMovement();
}