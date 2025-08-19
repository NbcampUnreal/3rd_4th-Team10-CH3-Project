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
	FTimerHandle ProjectileTimerHandle;
	int32 WeaponPower;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|FireData")
	UBoxComponent* ProjectileCollision;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	FVector ProjectileLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	FRotator ProjectileRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	FVector ProjectileDir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	float ProjectileSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	float ProjectileRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FireData")
	int32 TotalDamage;

	float LifeTime;
	bool Only;


    virtual void OnHit(UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit) override;

    virtual FVector SetHitScale() override;
	virtual void ProjectileMovement();

	void SetDamage(int32 WPower);
	void ProjectileLifeTime();


public:
	void Activate(ARangeWeapon* ActiveWeapon, FVector BulletPoint, FRotator BulletRotation, FVector FireDir);


	virtual void SetActive_Implementation(bool Active) override;
	virtual bool GetIsActive_Implementation() override;
	virtual void ActiveObject_Implementation() override;
	virtual void DeActiveObject_Implementation() override;
};
