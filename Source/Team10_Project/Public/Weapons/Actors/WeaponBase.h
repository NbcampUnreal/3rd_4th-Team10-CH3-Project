#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Interface/WeaponInterface.h"
#include "Items/ItemInterface.h"
#include "Items/ItemTypes.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
UCLASS()
class TEAM10_PROJECT_API AWeaponBase : public AActor, public IWeaponInterface, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	bool bIsEquip;
	bool bIsAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
	UStaticMeshComponent* WeaponStaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
	USkeletalMeshComponent* WeaponSkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RangeState")
	ERangeFireState FireState;

protected:
	TSet<AActor*> CollisionObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FVector Origin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FVector BoxExtent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FVector MeshSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	int32 Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	float Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	float Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	float Vertical;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
	UBoxComponent* GetCollision;

	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void GetItem() override;
	virtual void UseWeapon() override;
	virtual void EquipmentWeapon(AActor* Player) override;
	virtual void UnEquipmentWeapon(AActor* Player) override;
	virtual void Attack(AActor* Activator) override;
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

public:
	EWeaponType GetWeaponType() const;
	FName GetWeaponName() const;
	int32 GetPower() const;
};