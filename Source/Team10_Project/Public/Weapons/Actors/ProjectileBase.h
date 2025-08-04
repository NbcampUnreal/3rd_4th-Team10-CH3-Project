#pragma once

#include "CoreMinimal.h"
#include "Weapons/Actors/WeaponBase.h"
#include "Systems/Interface/PoolObjectInterface.h"
#include "ProjectileBase.generated.h"

class ARangeWeapon;

UCLASS()
class TEAM10_PROJECT_API AProjectileBase : public AWeaponBase, public IPoolObjectInterface
{
	GENERATED_BODY()
public:
	AProjectileBase();

private:
	bool bIsActive;
	FTimerHandle ProjectileTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	float ProjectileSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	float ProjectileRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|FireData")
	int32 TotalDamage;

	float LifeTime;
public:
	int32 WeaponPower;

protected:
	void OnHit(AActor* OtherActor);
public:
	void Activate(ARangeWeapon* ActiveWeapon, FVector BulletPoint);
	void SetDamage(int32 WPower);
	void ProjectileLifeTime(ARangeWeapon* ActiveWeapon);

	virtual bool GetIsActive_Implementation() override;
	virtual void ActiveObject_Implementation() override;
	virtual void DeActiveObject_Implementation() override;
};
