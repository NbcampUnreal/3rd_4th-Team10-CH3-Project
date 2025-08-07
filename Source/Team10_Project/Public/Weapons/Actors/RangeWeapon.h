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
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	ERangeLeverType LeverType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	ERangeFireType FireType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	float FireSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	int32 MaxBulletAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	int32 CurBulletAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	bool bIsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	FVector MuzzleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	FRotator MuzzleRotate;

	ERangeLeverType GetRangeLeverType();
	ERangeFireType GetAttackType();

	FTimerHandle FireTimerHandle;
	FTimerDelegate TimerDel;
public:
	virtual void Attack(AActor* Activator) override;

	void Reload(AActor* Activator);
	float GetFireSpeed();
	void SetFireState();
	void SwitchFireType();
};