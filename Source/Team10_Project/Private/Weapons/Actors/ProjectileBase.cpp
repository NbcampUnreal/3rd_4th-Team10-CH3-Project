#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "MyComponents/ObjectMovementComponent.h"

AProjectileBase::AProjectileBase()
	:ProjectileSpeed(0), ProjectileRange(0)
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
		&AProjectileBase::ProjectileDeActive,
		LifeTime,
		false
	);
}

void AProjectileBase::ProjectileDeActive()
{
	if (!bIsActive) return;

	if (bIsActive)
		return;//비활성화 기능 및 위치 초기화
}

