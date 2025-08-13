#include "Weapons/Actors/WeaponBase.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AWeaponBase::AWeaponBase()
	:bIsVisible(false), FireState(ERangeFireState::Idle), BoxExtent(FVector::ZeroVector), CollisionSize(FVector::ZeroVector),
    ItemType(EItemType::Weapon), WeaponType(), WeaponName(""), Power(0), RateOfFire(0.0f),
	Height(0.0f), Width(0.0f), Vertical(0.0f), bIsEquip(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponStaticMesh->SetupAttachment(Scene);
	
    GetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	GetCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GetCollision->SetupAttachment(Scene);
	GetCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnItemOverlap);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

    if (!bIsVisible)
    {
        InVisibleItem();
    }
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    OnItemOverlapJudgement();
}

void AWeaponBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlap Item"));
    UE_LOG(LogTemp, Warning, TEXT("OverlapActor : %s"), *OtherActor->GetName());
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

void AWeaponBase::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("EndOverlap Item"));
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

void AWeaponBase::OnItemOverlapJudgement()
{
    if (bOverlappingItem)
    {
        for(AMyPlayerController* InputController : OverlappingCharacters)
        {
            if (InputController->WasInputKeyJustPressed(EKeys::F))
            {
                AMyCharacter* MyChar = Cast<AMyCharacter>(InputController->GetPawn());
                if (MyChar)
                {
                    GetItem(MyChar);
                }
            }
        }
    }
}

void AWeaponBase::GetItem(AActor* Player)
{
    InVisibleItem();
    if (GetItemType() == EItemType::Weapon)
    {

    }
    else if (GetItemType() == EItemType::Buff)
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

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
}

void AWeaponBase::UseWeapon()
{
	
}

void AWeaponBase::EquipmentWeapon(AActor* Player)
{
    if (!Player) return;

    UE_LOG(LogTemp, Warning, TEXT("Equip"));
    AMyCharacter* Character = Cast<AMyCharacter>(Player);
    if (Character->GetCurrentWeapon())
    {
        UnEquipmentWeapon(Character);
    }

    FName GripSocketName = Character->GetWeaponSocketName();
    FTransform ArmGrips = Character->GetCharacterArms()->GetSocketTransform(GripSocketName, RTS_World);
    FTransform WeaponGrip = GetGripTransform();

    FTransform Offset = WeaponGrip.Inverse() * ArmGrips;
    FVector DesiredScale(0.75f);
    Offset.SetScale3D(DesiredScale);

    RootComponent->AttachToComponent(
        Character->GetCharacterArms(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        GripSocketName);

    WeaponStaticMesh->SetWorldTransform(Offset);
    Character->SetCurrentWeapon(this);
}

void AWeaponBase::UnEquipmentWeapon(AActor* Player)
{
    if (!Player) return;

    AMyCharacter* Character = Cast<AMyCharacter>(Player);
    Character->SetCurrentWeapon(NULL);
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

FTransform AWeaponBase::GetGripTransform() const
{
    FTransform GripTransform = WeaponStaticMesh->GetSocketTransform(TEXT("GripSocket"), RTS_Component);
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
