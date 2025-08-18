#pragma once

#include "CoreMinimal.h"
#include "Weapons/Actors/ProjectileBase.h"
#include "Bullet.generated.h"

UCLASS()
class TEAM10_PROJECT_API ABullet : public AProjectileBase
{
	GENERATED_BODY()
	
	ABullet();
    virtual void BeginPlay() override;
	virtual void ProjectileMovement() override;
};
