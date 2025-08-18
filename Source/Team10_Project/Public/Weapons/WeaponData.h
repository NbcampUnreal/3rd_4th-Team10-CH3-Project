#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PickupAmmo;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> ReloadTacMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> ReloadEmptyMontage;
};
