#include "Systems/HitBoxObject.h"
#include "Components/BoxComponent.h"
#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/WeaponBase.h"
#include "Systems/ObjectPoolComponent.h"

AHitBoxObject::AHitBoxObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	HitBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	HitBoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HitBoxCollision->SetupAttachment(Scene);

	HitBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHitBoxObject::Hit);
}

void AHitBoxObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHitBoxObject::HitBoxComp(AActor* Activator, float Height, float Width, float Vertical)
{
	HitBoxCollision->SetBoxExtent(FVector(Width, Vertical, Height));

	if (Cast<AWeaponBase>(Activator)->GetWeaponType() == EWeaponType::Projectile)
	{
		RootActor = Activator;
		HitBoxCollision->SetupAttachment(Cast<AWeaponBase>(Activator)->WeaponStaticMesh);
	}
}

void AHitBoxObject::Hit(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Cast<AProjectileBase>(RootActor)->OnHit(OtherActor);

	HitBoxCollision->SetupAttachment(Scene);
	UObjectPoolComponent* Pool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("UObjectPoolComponent"));
	Pool->ReturnObject(this);
}

bool AHitBoxObject::GetIsActive_Implementation()
{
	return bIsActive;
}

void AHitBoxObject::ActiveObject_Implementation()
{
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void AHitBoxObject::DeActiveObject_Implementation()
{
	if (!bIsActive) return;

	if (bIsActive)
	{
		bIsActive = false;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}