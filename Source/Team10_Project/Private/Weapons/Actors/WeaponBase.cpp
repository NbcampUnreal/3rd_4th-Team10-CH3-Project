#include "Weapons/Actors/WeaponBase.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"

AWeaponBase::AWeaponBase()
	:FireState(ERangeFireState::Idle), BoxExtent(FVector::ZeroVector), CollisionSize(FVector::ZeroVector),
	WeaponType(), WeaponName(""), Power(0), RateOfFire(0.0f),
	Height(0.0f), Width(0.0f), Vertical(0.0f), bIsEquip(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*GetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	GetCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GetCollision->SetupAttachment(Scene);

	GetCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnItemOverlap);*/
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Object"));
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//FŰ�� ������ ȹ�� �Ǵ� ��ȯ
		GetCollision->SetCollisionEnabled(ECollisionEnabled ::NoCollision);
	}
}

void AWeaponBase::GetItem()
{
}

void AWeaponBase::UseWeapon()
{
	
}

void AWeaponBase::EquipmentWeapon(AActor* Player)
{
    if (!Player) return;

    AMyCharacter* Character = Cast<AMyCharacter>(Player);
    FTransform GripTransform = Character->CharacterArms->GetSocketTransform(TEXT("MuzzleSocket"), RTS_World);
}

void AWeaponBase::UnEquipmentWeapon(AActor* Player)
{
    if (!Player) return;


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

void AWeaponBase::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	//if (!OtherActor) return;

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