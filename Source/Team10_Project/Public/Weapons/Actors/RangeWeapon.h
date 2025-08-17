#pragma once

#include "CoreMinimal.h"
#include "Weapons/Actors/WeaponBase.h"
#include "RangeWeapon.generated.h"

UCLASS()
class TEAM10_PROJECT_API ARangeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARangeWeapon();

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Effects|Camera")
    TSubclassOf<UCameraShakeBase> FireCameraShakeClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
    ERangeType RangeType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	ERangeLeverType LeverType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	ERangeFireType FireType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	float FireSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	int32 MaxBulletAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	int32 LoadAmmoAmount;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
    int32 ConsumeAmmoAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	bool bIsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	FVector MuzzleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	FRotator MuzzleRotate;

    virtual void Attack(AActor* Activator) override;
	ERangeLeverType GetRangeLeverType() const;
	ERangeFireType GetFireType() const;

    int32 FireCount;
    int32 RemainingFireCount;
    FTimerHandle FireCountHandle;
	FTimerHandle FireTimerHandle;
	FTimerDelegate TimerDel;

public:
    virtual void StartFire() override;
    virtual void StopFire() override;
	void Reload(AActor* Activator);
	float GetFireSpeed();
	void SetFireState();
	void SwitchFireType();

    int GetLoadedAmmoAmount() const;
    int GetMaxAmmoAmount() const;

};