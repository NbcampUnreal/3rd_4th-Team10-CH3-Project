#include "Weapons/Actors/WeaponBase.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AWeaponBase::AWeaponBase()
	:bIsVisible(false), FireState(ERangeFireState::Idle), BoxExtent(FVector::ZeroVector), CollisionSize(FVector::ZeroVector),
    ItemType(EItemType::Weapon), WeaponType(), WeaponName(""), Power(0), RateOfFire(0.0f),
	Height(0.0f), Width(0.0f), Vertical(0.0f), bIsEquip(false)
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponStaticMesh->SetupAttachment(Scene);

	GetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	GetCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GetCollision->SetupAttachment(Scene);

	GetCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnItemOverlap);
    GetCollision->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnItemEndOverlap);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
}

void AWeaponBase::OnItemEndOverlap(
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

void AWeaponBase::InteractiveItem(AActor* Player)
{
    if (GetItemType() == EItemType::Weapon)
    {

    }
}

void AWeaponBase::VisibleItem()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
}

void AWeaponBase::InVisibleItem()
{
    GetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCollision->Deactivate();
}

void AWeaponBase::UseWeapon()
{
	
}

void AWeaponBase::EquipmentWeapon(AActor* Player)
{
    if (!Player) return;

    AMyCharacter* Character = Cast<AMyCharacter>(Player);

    if (Character)
    {
        FName GripSocketName = Character->GetWeaponSocketName();

        FTransform ArmGripsSocket = Character->GetCharacterArms()->GetSocketTransform(GripSocketName, RTS_World);
        FTransform WeaponGripSocketW = GetGripTransform(RTS_World);

        FVector PivotToGrip = WeaponStaticMesh->GetComponentLocation() - WeaponGripSocketW.GetLocation();
        FVector DesiredLocation = ArmGripsSocket.GetLocation() + PivotToGrip;
        WeaponStaticMesh->SetWorldLocation(DesiredLocation, false, nullptr, ETeleportType::TeleportPhysics);

        FRotator ArmRotate = Character->GetCharacterArms()->GetRelativeRotation();
        FRotator WeaponGripRotate = GetGripTransform(RTS_Component).Rotator();
        FQuat SkeletalQuat = ArmGripsSocket.GetRotation() * WeaponGripSocketW.GetRotation().Inverse() * ArmRotate.Quaternion();
        WeaponStaticMesh->AddWorldRotation(SkeletalQuat);

        WeaponStaticMesh->AttachToComponent(
            Character->GetCharacterArms(),
            FAttachmentTransformRules::KeepWorldTransform,
            GripSocketName);
    }
}

void AWeaponBase::Attack(AActor* Activator)
{
	if (!Activator) return;
}

void AWeaponBase::StartFire()
{
}

void AWeaponBase::StopFire()
{
}

FVector AWeaponBase::SetHitScale()
{
	return FVector::ZeroVector;
}

FTransform AWeaponBase::GetGripTransform(ERelativeTransformSpace TransformSpace) const
{
    FTransform GripTransform = WeaponStaticMesh->GetSocketTransform(TEXT("GripSocket"), TransformSpace);
    return GripTransform;
}

void AWeaponBase::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor) return;

	if (OtherActor->ActorHasTag("Enemy"))
	{
		CollisionObject.Add(OtherActor);
	}
	else
	{
		//Enemy�� ������ �繰 ������Ʈ ex).��, ��, ���๰
	}
}

EWeaponType AWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

FName AWeaponBase::GetWeaponName() const
{
	return WeaponName;
}

int32 AWeaponBase::GetPower() const
{
	return Power;
}

EItemType AWeaponBase::GetItemType()
{
    return ItemType;
}

bool AWeaponBase::GetItemOverlapState()
{
    return bOverlappingItem;
}
