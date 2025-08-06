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

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	AActor* RootActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitComponentData")
	UBoxComponent* HitBoxCollision;

	void HitBoxComp(AActor* Activator, float Height, float Width, float Vertical);
	void Hit(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual bool GetIsActive_Implementation() override;
	virtual void ActiveObject_Implementation() override;
	virtual void DeActiveObject_Implementation() override;
};