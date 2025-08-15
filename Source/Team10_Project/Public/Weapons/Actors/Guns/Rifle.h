#pragma once

#include "CoreMinimal.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "Rifle.generated.h"

UCLASS()
class TEAM10_PROJECT_API ARifle : public ARangeWeapon
{
	GENERATED_BODY()

public:
    ARifle();

    virtual void BeginPlay() override;
    virtual void StartFire() override;
};
