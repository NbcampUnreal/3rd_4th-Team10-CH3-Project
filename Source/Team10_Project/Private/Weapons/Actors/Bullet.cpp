#include "Weapons/Actors/Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	WeaponName = "Bullet";
	ProjectileSpeed = 50.0f;
	ProjectileRange = 8000.0f;
	Power = 10;

	WeaponStaticMesh->GetLocalBounds(Origin, BoxExtent);
	MeshSize = BoxExtent * 2.f;
	Height = MeshSize.Z;
	Width = MeshSize.X;
	Vertical = MeshSize.Y;
}

void ABullet::ProjectileMovement()
{
	Super::ProjectileMovement();

	UE_LOG(LogTemp, Warning, TEXT("Move"));

	ProjectileMovementComp->Velocity = ProjectileDir * ProjectileSpeed;
	ProjectileCollision->Activate();
	ProjectileMovementComp->Activate(true);

	ProjectileLifeTime();
}