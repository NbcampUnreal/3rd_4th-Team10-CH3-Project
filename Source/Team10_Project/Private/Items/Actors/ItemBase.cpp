#include "Items/Actors/ItemBase.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    InteractiveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    InteractiveCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    InteractiveCollision->SetupAttachment(Scene);

    ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
    ItemStaticMesh->SetupAttachment(InteractiveCollision);
    ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    InteractiveCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnItemOverlap);
    InteractiveCollision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnItemEndOverlap);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp, 
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, 
    bool bFromSweep, 
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        if (AMyCharacter* MyChar = Cast<AMyCharacter>(OtherActor))
        {
            if (AMyPlayerController* MyPlayerCon = Cast<AMyPlayerController>(MyChar->GetController()))
            {
                OverlappingCharacters.Add(MyPlayerCon);
            }
        }

        if (!OverlappingCharacters.IsEmpty())
        {
            bOverlappingItem = true;
        }
    }
    else
    {
        return;
    }
}

void AItemBase::OnItemEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        if (AMyCharacter* MyChar = Cast<AMyCharacter>(OtherActor))
        {
            if (AMyPlayerController* MyPlayerCon = Cast<AMyPlayerController>(MyChar->GetController()))
            {
                OverlappingCharacters.Remove(MyPlayerCon);
            }
        }

        if (OverlappingCharacters.IsEmpty())
        {
            bOverlappingItem = false;
        }
    }
}

void AItemBase::InteractiveItem(AActor* Activate)
{

}

void AItemBase::VisibleItem()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
}

void AItemBase::InVisibleItem()
{
    InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InteractiveCollision->Deactivate();

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);

    OverlappingCharacters.Empty();
}

void AItemBase::DestoryItem()
{
    Destroy();
}

EItemType AItemBase::GetItemType()
{
    return ItemType;
}

bool AItemBase::GetItemOverlapState()
{
    return bOverlappingItem;
}

