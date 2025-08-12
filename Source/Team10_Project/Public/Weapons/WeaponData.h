#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponDataType : uint8
{
    Pistol UMETA(DisplayName = "Pistol"),
    Rifle UMETA(DisplayName = "Rifle"),
    Shotgun UMETA(DisplayName = "Shotgun")
};
class AWeaponBase;

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AWeaponBase> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAmmo;
};
