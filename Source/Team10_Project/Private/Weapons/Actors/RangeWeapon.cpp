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

        RemainingFireCount--;
        LoadAmmoAmount -= ConsumeAmmoAmount;
        UE_LOG(LogTemp, Warning, TEXT("LoadAmmo Amount: %d"), LoadAmmoAmount);

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
        FireCount = 99;
    }
}

void ARangeWeapon::StopFire()
{
    GetWorld()->GetTimerManager().ClearTimer(FireCountHandle);
}

void ARangeWeapon::Reload(AActor* Activator)
{
	if (!Activator) return;

	FireState = ERangeFireState::Reload;
	int32 RemainingAmmo = 10;
    AMyCharacter* Character = Cast<AMyCharacter>(Activator);

    int32 RemainingBullet = Character->GetAmmoAmount();
    if (RemainingAmmo > 0 && MaxBulletAmount > LoadAmmoAmount)
    {
        int32 NeedAmmo = MaxBulletAmount - LoadAmmoAmount;
        if (RemainingAmmo >= NeedAmmo)
        {
            LoadAmmoAmount += NeedAmmo;
            Character->SetAmmoAmount(-NeedAmmo);
        }
        else if (RemainingAmmo < NeedAmmo)
        {
            LoadAmmoAmount += RemainingAmmo;
            Character->SetAmmoAmount(-RemainingAmmo);
        }
        bIsFire = true;
        FireState = ERangeFireState::Load;
    }
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
	FireState = ERangeFireState::Load;

    /*AMyCharacter* Character = nullptr;
    for (TActorIterator<AMyCharacter> It(GetWorld()); It; ++It)
    {
        Character = *It;
        break;
    }
    EquipmentWeapon(Character);*/
}

DEFINE_LOG_CATEGORY_STATIC(LogRangeWeapon, Log, All);

static FORCEINLINE FString EnumNameString(ERangeFireType V)
{
    return StaticEnum<ERangeFireType>()->GetNameStringByValue(static_cast<int64>(V));
}

static FORCEINLINE FString EnumDisplayString(ERangeFireType V)
{
    return StaticEnum<ERangeFireType>()->GetDisplayNameTextByValue(static_cast<int64>(V)).ToString();
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

    UE_LOG(LogRangeWeapon, Log, TEXT("FireType: %s (%s)"),
        *EnumNameString(GetFireType()),
        *EnumDisplayString(GetFireType()));
}

int ARangeWeapon::GetLoadedAmmoAmount() const
{
	return LoadAmmoAmount;
}

int ARangeWeapon::GetMaxAmmoAmount() const
{
    return MaxBulletAmount;
}
