#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemInterface.h"
#include "ItemBase.generated.h"

class UBoxComponent;
class AMyPlayerController;

UCLASS()
class TEAM10_PROJECT_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()

protected:
    AItemBase();

	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    UBoxComponent* InteractiveCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    UStaticMeshComponent* ItemStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemnData")
    FName ItemName;


    bool bOverlappingItem;
    TArray<AMyPlayerController*> OverlappingCharacters;

    virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, 
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp, 
        int32 OtherBodyIndex,
        bool bFromSweep, 
        const FHitResult& SweepResult) override;
    virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) override;
    virtual void InteractiveItem(AActor*) override;
    virtual void VisibleItem() override;
    virtual void InVisibleItem() override;
    virtual void DestoryItem();
    virtual EItemType GetItemType() override;
    virtual bool GetItemOverlapState() override;
};
