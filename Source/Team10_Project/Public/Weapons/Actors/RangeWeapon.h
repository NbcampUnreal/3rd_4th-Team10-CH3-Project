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

    virtual void BeginPlay() override;
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

    UPROPERTY(EditAnywhere, Category = "Weapon | Audio")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, Category = "Weapon | Audio")
    USoundBase* ReloadTacSound;

    UPROPERTY(EditAnywhere, Category = "Weapon | Audio")
    USoundBase* ReloadEmptySound;

    virtual void Attack(AActor* Activator) override;

    int32 FireCount;
    int32 RemainingFireCount;
    FTimerHandle FireCountHandle;
	FTimerHandle FireTimerHandle;
	FTimerDelegate TimerDel;

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

    ERangeType GetRangeType() const;
	ERangeLeverType GetRangeLeverType() const;
	ERangeFireType GetFireType() const;

	float GetFireSpeed();
	void Reload(AActor* Activator);
	void SetFireState(bool IsFire, ERangeFireState CurFireState);
	void SwitchFireType();

    UFUNCTION(BlueprintCallable, Category = "RangeWeaponData")
    FString GetFireTypeString();
    UFUNCTION(BlueprintCallable, Category = "RangeWeaponData")
    int GetLoadedAmmoAmount() const;
    UFUNCTION(BlueprintCallable, Category = "RangeWeaponData")
    int GetMaxAmmoAmount() const;
};