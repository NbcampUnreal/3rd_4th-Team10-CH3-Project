#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Buff UMETA(DisplayName = "Buff")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Projectile UMETA(DisplayName = "Projectile"),
	Throw UMETA(DisplayName = "Throw")
};

UENUM(BlueprintType)
enum class ERangeLeverType : uint8
{
	Single UMETA(DisplayName = "Weapon"),
	Point UMETA(DisplayName = "Point"),
	Repeatedly UMETA(DisplayName = "Repeatedly")
};

UENUM(BlueprintType)
enum class ERangeFireType : uint8
{
	SingleShot UMETA(DisplayName = "Weapon"),
	PointFire UMETA(DisplayName = "Point"),
	Repeatedly UMETA(DisplayName = "Repeatedly")
};

UENUM(BlueprintType)
enum class ERangeFireState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Load UMETA(DisplayName = "Load"),
	Reload UMETA(DisplayName = "Reload"),
	Fire UMETA(DisplayName = "Fire")
};

UENUM(BlueprintType)
enum class EThrowAttackType : uint8
{
	Bomb UMETA(DisplayName = "Bomb"),
	Shot UMETA(DisplayName = "Shot")
};

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	Heal UMETA(DisplayName = "Heal"),
	Speed UMETA(DisplayName = "Speed")
};

UENUM(BlueprintType)
enum class EItemUsableType: uint8
{
    Immediately UMETA(DisplayName = "Immediately"),
    Acquire UMETA(DisplayName = "Acquire")
};

class TEAM10_PROJECT_API ItemTypes
{

};
