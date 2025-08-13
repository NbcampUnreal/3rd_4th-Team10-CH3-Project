#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Interface/WeaponInterface.h"
#include "Items/ItemInterface.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class AMyPlayerController;
UCLASS()
class TEAM10_PROJECT_API AWeaponBase : public AActor, public IWeaponInterface, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

private:
    bool bOverlappingItem;
    TArray<AMyPlayerController*> OverlappingCharacters;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	bool bIsEquip;
	bool bIsAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RangeState")
	ERangeFireState FireState;

protected:
    bool bIsVisible;
	TSet<AActor*> CollisionObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
	UStaticMeshComponent* WeaponStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FVector BoxExtent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FVector CollisionSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
    EItemType ItemType;
	
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

    // Overlap Hit Event
    virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnHit(UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit) override;

    //ItemInterface
    virtual void OnItemOverlapJudgement() override;
    virtual void GetItem(AActor* Player) override;
    virtual void VisibleItem() override;
    virtual void InVisibleItem() override;

    //Weapon
	virtual void UseWeapon() override;
    virtual FVector SetHitScale() override;
    FTransform GetGripTransform() const;

public:
    //WeaponInterface
	virtual void EquipmentWeapon(AActor* Player) override;
	virtual void UnEquipmentWeapon(AActor* Player) override;
	virtual void Attack(AActor* Activator) override;
    virtual void StartFire() override;
    virtual void StopFire() override;

	EWeaponType GetWeaponType() const;
	FName GetWeaponName() const;
	int32 GetPower() const;

    virtual EItemType GetItemType() override;
};