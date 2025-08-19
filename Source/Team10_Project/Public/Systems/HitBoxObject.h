#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Systems/Interface/PoolObjectInterface.h"
#include "HitBoxObject.generated.h"

class UBoxComponent;

UCLASS()
class TEAM10_PROJECT_API AHitBoxObject : public AActor, public IPoolObjectInterface
{
	GENERATED_BODY()
	
public:	
	AHitBoxObject();
	
private:
	FTimerHandle HitBoxTimerHandle;
	float LifeTime;

    int32 Damage;

    TSet<AActor*> HitObject;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	UBoxComponent* HitBoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	TSet<AActor*> HitObjectSet;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	bool bIsActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	bool Only;

	void HitBoxComp(AActor* Activator, float Height, float Width, float Vertical, float Time, bool Only);
	UFUNCTION()
	void OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void HitBoxLifeTime(float Time);
    void SetDamage(int32 NewDamage);
    UFUNCTION(BlueprintPure, Category = "HitComponentData")
    int32 GetDamage() const;

	virtual void SetActive_Implementation(bool Active) override;
	virtual bool GetIsActive_Implementation() override;
	virtual void ActiveObject_Implementation() override;
	virtual void DeActiveObject_Implementation() override;
};
