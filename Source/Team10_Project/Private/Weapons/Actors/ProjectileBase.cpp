#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "MyComponents/ObjectMovementComponent.h"
#include "Systems/ObjectPoolManager.h"
#include "Systems/HitboxObject.h"

AProjectileBase::AProjectileBase()
	:ProjectileSpeed(0), ProjectileRange(0)
{
	WeaponType = EWeaponType::Projectile;
}

void AProjectileBase::Activate(ARangeWeapon* ActiveWeapon, FVector BulletPoint, FRotator BulletRotation)
{
	this->SetActorLocation(BulletPoint);
	this->SetActorRotation(BulletRotation);

	UObjectPoolManager* Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	AHitBoxObject* HitBox = Pool->GetObject<AHitBoxObject>();
	HitBox->HitBoxComp(this, Height, Width, Vertical);
	SetDamage(ActiveWeapon->GetPower());

	UObjectMovementComponent* ObjMoveComponent = CreateDefaultSubobject<UObjectMovementComponent>(TEXT("UObjectMovementComponent"));
	ObjMoveComponent->AddFrontMovementComp(this, ProjectileSpeed);
	ProjectileLifeTime(ActiveWeapon);
}

void AProjectileBase::OnHit(AActor* CollisionActor)
{
	Super::OnHit(CollisionActor);

	if (CollisionActor->ActorHasTag("Enemy"))
	{
		//Cast<ACharacter_Monster>(CollisionActor)->TakeDamage(TotalDamage);
	}

	UObjectPoolManager* Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	Pool->ReturnObject(this);
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

