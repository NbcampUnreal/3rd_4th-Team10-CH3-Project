#include "Weapons/Actors/WeaponBase.h"
#include "Components/BoxComponent.h"

AWeaponBase::AWeaponBase()
	:WeaponType(), WeaponName(""), Power(0), RateOfFire(0.0f),
	Height(0.0f), Width(0.0f), Vertical(0.0f), FireState(ERangeFireState::Idle), bIsEquip(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	WeaponMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnItemOverlap);
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
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//F키를 누르면 획득 또는 교환
		Collision->SetCollisionEnabled(ECollisionEnabled ::NoCollision);
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
}

void AWeaponBase::UnEquipmentWeapon(AActor* Player)
{
}

void AWeaponBase::Attack(AActor* Activator)
{
	if (!Activator) return;
}

void AWeaponBase::OnHit(AActor* CollisionActor)
{
	if (!CollisionActor) return;

	if (CollisionActor->ActorHasTag("Enemy"))
	{
		CollisionObject.Add(CollisionActor);
	}
	else
	{
		//Enemy를 제외한 사물 오브젝트 ex).땅, 벽, 건축물
	}
}

EWeaponType AWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

int32 AWeaponBase::GetPower() const
{
	return Power;
}