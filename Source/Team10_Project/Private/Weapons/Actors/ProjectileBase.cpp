#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "MyComponents/ObjectMovementComponent.h"
#include "Systems/ObjectPoolComponent.h"

AProjectileBase::AProjectileBase()
	:ProjectileSpeed(0), ProjectileRange(0), bIsActive(true)
{
}

void AProjectileBase::Activate(ARangeWeapon* ActiveWeapon, FVector BulletPoint)
{
	UObjectMovementComponent* ObjMoveComponent = CreateDefaultSubobject<UObjectMovementComponent>(TEXT("UObjectMovementComponent"));
	SetDamage(ActiveWeapon->GetPower());
	this->SetActorLocation(BulletPoint);
	ObjMoveComponent->AddFrontMovement(this, ProjectileSpeed);
	ProjectileLifeTime(ActiveWeapon);
}

void AProjectileBase::OnHit(AActor* CollisionActor)
{
	Super::OnHit(CollisionActor);

	UObjectPoolComponent* Pool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("UObjectPoolComponent"));
	Pool->ReturnObject(this);
	//비활성화 기능 추가
}

void AProjectileBase::SetDamage(int32 WPower)
{
	TotalDamage = Power + WPower;
}

void AProjectileBase::ProjectileLifeTime(ARangeWeapon* ActiveWeapon)
{
	ActiveWeapon->GetFireSpeed();
	LifeTime = ProjectileRange / ProjectileSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		ProjectileTimerHandle,
		this,
		&AProjectileBase::DeActiveObject,
		LifeTime,
		false
	);
}

bool AProjectileBase::GetIsActive_Implementation()
{
	return bIsActive;
}

void AProjectileBase::ActiveObject_Implementation()
{
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void AProjectileBase::DeActiveObject_Implementation()
{
	if (!bIsActive) return;

	if (bIsActive)
	{
		bIsActive = false;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}

