#include "Items/Actors/ItemBase.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    GetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    GetCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    GetCollision->SetupAttachment(Scene);

    GetCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnItemOverlap);
    GetCollision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnItemEndOverlap);
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
    UE_LOG(LogTemp, Warning, TEXT("OnOverlap Item"));
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
    UE_LOG(LogTemp, Warning, TEXT("EndOverlap Item"));
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
    GetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCollision->Deactivate();

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
}

EItemType AItemBase::GetItemType()
{
    return ItemType;
}

bool AItemBase::GetItemOverlapState()
{
    return bOverlappingItem;
}

