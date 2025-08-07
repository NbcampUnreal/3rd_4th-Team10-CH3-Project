#pragma once

#include "CoreMinimal.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "Pistol.generated.h"

UCLASS()
class TEAM10_PROJECT_API APistol : public ARangeWeapon
{
	GENERATED_BODY()
public:
	APistol();

	virtual void BeginPlay() override;
};
