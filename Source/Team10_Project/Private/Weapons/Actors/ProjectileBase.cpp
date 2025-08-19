#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "Systems/ObjectPoolManager.h"
#include "Systems/HitboxObject.h"
#include "AI/Character_Monster.h"
#include "MyCharacter.h"

#include "Components/BoxComponent.h"
#include "EngineUtils.h"

AProjectileBase::AProjectileBase()
	:bIsActive(false), ProjectileLocation(FVector::ZeroVector), ProjectileRotation(FRotator::ZeroRotator), ProjectileDir(FVector::ZeroVector), 
    ProjectileSpeed(0), ProjectileRange(0), Only(false)
{
	WeaponType = EWeaponType::Projectile;

    GetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    SetRootComponent(ProjectileCollision);
    ProjectileCollision->Deactivate();

	WeaponStaticMesh->SetupAttachment(ProjectileCollision);

	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

void AProjectileBase::Activate(ARangeWeapon* ActiveWeapon, FVector ProjectileLoc, FRotator ProjectileRotate, FVector FireDir)
{
    ProjectileCollision->SetRelativeLocation(FVector::ZeroVector);
    WeaponStaticMesh->SetRelativeLocation(FVector::ZeroVector);

    ProjectileLocation = ProjectileLoc;
	ProjectileRotation = ProjectileRotate;
	ProjectileDir = FireDir;

    SetActorLocationAndRotation(ProjectileLocation, ProjectileRotation, false, nullptr, ETeleportType::TeleportPhysics);
    ProjectileCollision->SetWorldLocationAndRotation(ProjectileLocation, ProjectileRotation);

	SetDamage(ActiveWeapon->GetPower());

	ProjectileMovement();
}

void AProjectileBase::ProjectileMovement()
{
    ProjectileCollision->SetPhysicsLinearVelocity(ProjectileDir * ProjectileSpeed);
	ProjectileLifeTime();
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

    if (OtherActor == GetInstigator() || OtherActor->GetOwner() == GetOwner() 
        || OtherActor->GetOwner() == GetInstigator() || OtherActor->GetInstigator() == GetOwner())
    {
        return;
    }

    AObjectPoolManager* Pool = nullptr;
    for (TActorIterator<AObjectPoolManager> It(GetWorld()); It; ++It)
    {
        Pool = *It;
        break;
    }

    if (OtherActor->ActorHasTag("Enemy"))
    {
        SetHitScale();
        AHitBoxObject* HitBox = Pool->GetObject<AHitBoxObject>();
        HitBox->SetDamage(TotalDamage);
        HitBox->SetOwner(GetOwner());
        HitBox->SetInstigator(GetInstigator());
        HitBox->HitBoxComp(this, Height, Width, Vertical, LifeTime, Only);
    }

    Pool->ReturnObject(this);
}

void AProjectileBase::SetDamage(int32 WPower)
{
	TotalDamage = Power + WPower;
}

void AProjectileBase::ProjectileLifeTime()
{
    LifeTime = ProjectileRange / ProjectileSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		ProjectileTimerHandle,
		this,
		&AProjectileBase::DeActiveObject_Implementation,
		LifeTime,
		false
	);
}

FVector AProjectileBase::SetHitScale()
{
    BoxExtent = ProjectileCollision->GetScaledBoxExtent();
    CollisionSize = BoxExtent;
    Height = CollisionSize.Z;
    Width = CollisionSize.X;
    Vertical = CollisionSize.Y;

    FVector HitScale = { Height, Width, Vertical };
    return HitScale;
}

void AProjectileBase::SetActive_Implementation(bool Active)
{
	bIsActive = Active;
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
    
	ProjectileCollision->Activate();
	ProjectileCollision->SetNotifyRigidBodyCollision(true);
    ProjectileCollision->SetSimulatePhysics(true);
}

void AProjectileBase::DeActiveObject_Implementation()
{
	if (!bIsActive) return;

	if (bIsActive)
	{
        ProjectileCollision->SetPhysicsLinearVelocity(FVector::ZeroVector);
        ProjectileCollision->SetSimulatePhysics(false);

		bIsActive = false;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		ProjectileCollision->SetNotifyRigidBodyCollision(false);
		ProjectileCollision->Deactivate();

        ProjectileCollision->SetWorldLocation(FVector::ZeroVector);

        SetActorLocation(FVector::ZeroVector);
        CollisionObject.Empty();
	}
}

