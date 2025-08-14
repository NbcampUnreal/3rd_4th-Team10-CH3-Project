#include "Weapons/Actors/RangeWeapon.h"
#include "Weapons/Actors/ProjectileBase.h"
#include "Systems/ObjectPoolManager.h"
#include "EngineUtils.h"

#include "MyCharacter.h"
ARangeWeapon::ARangeWeapon()
	:LeverType(ERangeLeverType::Single), FireType(ERangeFireType::SingleShot),
	FireSpeed(0), MaxBulletAmount(0), LoadAmmoAmount(0), ConsumeAmmoAmount(0), bIsFire(true), MuzzleLocation(FVector::ZeroVector), MuzzleRotate(FRotator::ZeroRotator)
{
	WeaponType = EWeaponType::Range;

	WeaponStaticMesh->SetupAttachment(Scene);
}

void ARangeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (LeverType == ERangeLeverType::Single)
	{
		FireType = ERangeFireType::SingleShot;
	}
	if (LeverType == ERangeLeverType::Point)
	{
		FireType = ERangeFireType::PointFire;
	}
	if (LeverType == ERangeLeverType::Repeatedly)
	{
		FireType = ERangeFireType::Repeatedly;
	}
}

void ARangeWeapon::Attack(AActor* Activator)
{
	Super::Attack(Activator);

	if (bIsFire && FireState == ERangeFireState::Load)
	{
		UE_LOG(LogTemp, Warning, (TEXT("Fire")));
		FireState = ERangeFireState::Fire;

		FTransform SocketWorldTransform = WeaponStaticMesh->GetSocketTransform(TEXT("MuzzleSocket"), RTS_World);

		FVector FireDirection = SocketWorldTransform.GetRotation().GetForwardVector().GetSafeNormal();
		MuzzleLocation = SocketWorldTransform.GetLocation();
		MuzzleRotate = FireDirection.Rotation();

        AObjectPoolManager* Pool = nullptr;
        for (TActorIterator<AObjectPoolManager> It(GetWorld()); It; ++It)
        {
            Pool = *It;
            break;
        }
		AProjectileBase* Projectile = Pool->GetObject<ABullet>();
		Projectile->Activate(this, MuzzleLocation, MuzzleRotate, FireDirection);
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			this,
			&ARangeWeapon::SetFireState,
			RateOfFire,
			false
		);

        LoadAmmoAmount -= ConsumeAmmoAmount;
		//장전된 총알이 다 떨어졌으면
		if (LoadAmmoAmount == 0)
		{
			bIsFire = false;
			FireState = ERangeFireState::Idle;
		}
	}
}
void ARangeWeapon::StartFire()
{
}

void ARangeWeapon::StopFire()
{
}

void ARangeWeapon::Reload(AActor* Activator)
{
	if (!Activator) return;

	FireState = ERangeFireState::Reload;
	int32 RemainingAmmo = 10;
    AMyCharacter* Character = nullptr;
    for (TActorIterator<AMyCharacter> It(GetWorld()); It; ++It)
    {
        Character = *It;
        break;
    }

    //int32 RemainingBullet = Character->GetPossessAmmo();
    if (RemainingAmmo > 0 && MaxBulletAmount > LoadAmmoAmount)
    {
        int32 NeedAmmo = MaxBulletAmount - LoadAmmoAmount;
        if (RemainingAmmo >= NeedAmmo)
        {
            LoadAmmoAmount += NeedAmmo;
            //Character->SetRemainingAmmo(-NeedAmmo);
        }
        else if (RemainingAmmo < NeedAmmo)
        {
            LoadAmmoAmount += RemainingAmmo;
            //Character->SetRemainingAmmo(-RemainingAmmo);
        }
        bIsFire = true;
        FireState = ERangeFireState::Load;
    }
}

ERangeLeverType ARangeWeapon::GetRangeLeverType()
{
	return LeverType;
}

ERangeFireType ARangeWeapon::GetAttackType()
{
	return FireType;
}

float ARangeWeapon::GetFireSpeed()
{
	return FireSpeed;
}

void ARangeWeapon::SetFireState()
{
	FireState = ERangeFireState::Load;
}

void ARangeWeapon::SwitchFireType()
{
	if (FireType == ERangeFireType::SingleShot)
	{
		if (LeverType == ERangeLeverType::Point)
		{
			FireType = ERangeFireType::PointFire;
		}
		if (LeverType == ERangeLeverType::Repeatedly)
		{
			FireType = ERangeFireType::Repeatedly;
		}
	}
	else if (FireType == ERangeFireType::PointFire)
	{
		FireType = ERangeFireType::SingleShot;
	}
	else if (FireType == ERangeFireType::Repeatedly)
	{
		FireType = ERangeFireType::SingleShot;
	}
}

int ARangeWeapon::GetLoadedAmmoAmount() const
{
	return LoadAmmoAmount;
}

int ARangeWeapon::GetMaxAmmoAmount() const
{
    return MaxBulletAmount;
}
