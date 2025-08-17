#include "Weapons/Actors/RangeWeapon.h"
#include "Weapons/Actors/ProjectileBase.h"
#include "Systems/ObjectPoolManager.h"
#include "EngineUtils.h"

#include "MyCharacter.h"
ARangeWeapon::ARangeWeapon()
	:RangeType(ERangeType::None), LeverType(ERangeLeverType::Single), FireType(ERangeFireType::SingleShot),
	FireSpeed(0), MaxBulletAmount(0), LoadAmmoAmount(0), ConsumeAmmoAmount(0), bIsFire(true), MuzzleLocation(FVector::ZeroVector), MuzzleRotate(FRotator::ZeroRotator), FireCount(1), RemainingFireCount(0)
{
	WeaponType = EWeaponType::Range;

	WeaponStaticMesh->SetupAttachment(Scene);
}

void ARangeWeapon::Attack(AActor* Activator)
{
	Super::Attack(Activator);

	if (bIsFire && FireState == ERangeFireState::Load && RemainingFireCount > 0)
	{
		FireState = ERangeFireState::Fire;
        bIsFire = false;

		FTransform SocketWorldTransform = GetGripTransform(RTS_World);

		FVector FireDirection = SocketWorldTransform.GetRotation().GetForwardVector().GetSafeNormal();
		MuzzleLocation = SocketWorldTransform.GetLocation();
		MuzzleRotate = FireDirection.Rotation();

        if (FireCameraShakeClass)
        {
            if (AMyCharacter* Player = Cast<AMyCharacter>(Activator->GetOwner()))
            {
                if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
                {
                    if (PlayerController)
                    {
                        PlayerController->ClientStartCameraShake(FireCameraShakeClass);
                    }
                }
            }
        }

        AObjectPoolManager* Pool = nullptr;
        for (TActorIterator<AObjectPoolManager> It(GetWorld()); It; ++It)
        {
            Pool = *It;
            break;
        }
		AProjectileBase* Projectile = Pool->GetObject<ABullet>();
        Projectile->SetOwner(Activator);
        Projectile->SetInstigator(Cast<AMyCharacter>(Activator->GetOwner()));
		Projectile->Activate(this, MuzzleLocation, MuzzleRotate, FireDirection);
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			this,
			&ARangeWeapon::SetFireState,
			RateOfFire,
			false
		);

        RemainingFireCount--;
        LoadAmmoAmount -= ConsumeAmmoAmount;
        UE_LOG(LogTemp, Warning, TEXT("LoadAmmo Amount: %d"), GetLoadedAmmoAmount());

        if (RemainingFireCount == 0)
        {
            StopFire();
        }
		//장전된 총알이 다 떨어졌으면
		if (LoadAmmoAmount == 0)
		{
            StopFire();
            bIsFire = false;
			FireState = ERangeFireState::Idle;
		}
	}
}
void ARangeWeapon::StartFire()
{
    if (FireType == ERangeFireType::SingleShot)
    {
        FireCount = 1;
    }
    else if (FireType == ERangeFireType::PointFire)
    {
        FireCount = 3;
    }
    else if (FireType == ERangeFireType::Repeatedly)
    {
        FireCount = MaxBulletAmount;
    }
}

void ARangeWeapon::StopFire()
{
    if (FireState == ERangeFireState::Fire)
    {
        bIsFire = true;
        FireState = ERangeFireState::Load;
        GetWorld()->GetTimerManager().ClearTimer(FireCountHandle);
    }
}

void ARangeWeapon::Reload(AActor* Activator)
{
	if (!Activator) return;

	FireState = ERangeFireState::Reload;
    AMyCharacter* Character = Cast<AMyCharacter>(Activator);

    int32 RemainingBullet = Character->GetAmmoAmount();
    if (RemainingBullet > 0 && MaxBulletAmount > LoadAmmoAmount)
    {
        int32 NeedAmmo = MaxBulletAmount - LoadAmmoAmount;
        if (RemainingBullet >= NeedAmmo)
        {
            LoadAmmoAmount += NeedAmmo;
            Character->SetAmmoAmount(-NeedAmmo);
        }
        else if (RemainingBullet < NeedAmmo)
        {
            LoadAmmoAmount += RemainingBullet;
            Character->SetAmmoAmount(-RemainingBullet);
        }
        bIsFire = true;
        FireState = ERangeFireState::Load;
    }
    UE_LOG(LogTemp, Warning, TEXT("GetAmmo %d"), Character->GetAmmoAmount());
}

ERangeType ARangeWeapon::GetRangeType() const
{
    return RangeType;
}

ERangeLeverType ARangeWeapon::GetRangeLeverType() const
{
	return LeverType;
}

ERangeFireType ARangeWeapon::GetFireType() const
{
	return FireType;
}

float ARangeWeapon::GetFireSpeed()
{
	return FireSpeed;
}

void ARangeWeapon::SetFireState()
{
    bIsFire = true;
	FireState = ERangeFireState::Load;
}

void ARangeWeapon::SwitchFireType()
{
    if (FireType == ERangeFireType::SingleShot)
    {
        if (LeverType == ERangeLeverType::Single)
        {
            return;
        }
        else if (LeverType == ERangeLeverType::Point)
        {
            FireType = ERangeFireType::PointFire;
        }
        else if (LeverType == ERangeLeverType::Repeatedly)
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

FString ARangeWeapon::GetFireTypeString()
{
    FString FireTypeString;
    
    if (FireType == ERangeFireType::SingleShot)
    {
        FireTypeString = TEXT("Single Shot");
    }
    else if (FireType == ERangeFireType::PointFire)
    {
        FireTypeString = TEXT("Point Fire");
    }
    else if (FireType == ERangeFireType::Repeatedly)
    {
        FireTypeString = TEXT("Repeatedly");
    }

    return FireTypeString;
}

int ARangeWeapon::GetLoadedAmmoAmount() const
{
	return LoadAmmoAmount;
}

int ARangeWeapon::GetMaxAmmoAmount() const
{
    return MaxBulletAmount;
}
