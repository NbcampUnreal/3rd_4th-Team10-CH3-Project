#include "Weapons/Actors/WeaponBase.h"
#include "Components/BoxComponent.h"

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
		//F키를 누르면 획득 또는 교환
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


}

void AWeaponBase::UnEquipmentWeapon(AActor* Player)
{
    if (!Player) return;
}

void AWeaponBase::Attack(AActor* Activator)
{
	if (!Activator) return;
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
		//Enemy를 제외한 사물 오브젝트 ex).땅, 벽, 건축물
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