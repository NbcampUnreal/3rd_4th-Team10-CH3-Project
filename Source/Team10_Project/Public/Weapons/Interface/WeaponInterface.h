#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM10_PROJECT_API IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void UseWeapon() = 0;
	virtual void EquipmentWeapon(AActor* Player) = 0;
	virtual void Attack(AActor* Activator) = 0;
    virtual void StartFire() = 0;
    virtual void StopFire() = 0;
    virtual FVector SetHitScale() = 0;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) = 0;
};
