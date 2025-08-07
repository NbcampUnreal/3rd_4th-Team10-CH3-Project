#include "Systems/HitBoxObject.h"
#include "Components/BoxComponent.h"
#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/WeaponBase.h"
#include "Systems/ObjectPoolManager.h"

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

void AHitBoxObject::Tick(float Time)
{
	Super::Tick(Time);
	FVector Center = HitBoxCollision->GetComponentLocation();
	FVector Extent = HitBoxCollision->GetScaledBoxExtent(); // Scale 반영된 실제 크기
	FRotator Rotation = HitBoxCollision->GetComponentRotation();

	DrawDebugBox(
		GetWorld(),
		Center,
		Extent,
		Rotation.Quaternion(),
		FColor::Red,
		true,     // 지속 여부
		2.f,       // 지속 시간 (0이면 한 프레임만)
		0,         // Depth Priority
		2.f        // 선 굵기
	);
}

void AHitBoxObject::HitBoxComp(AActor* Activator, float Height, float Width, float Vertical, float Time)
{
	HitBoxCollision->SetBoxExtent(FVector(Width, Vertical, Height));

	AWeaponBase* Weapon  = Cast<AWeaponBase>(Activator);
	if (Weapon->GetWeaponType() == EWeaponType::Projectile)
	{
		RootActor = Activator;

		HitBoxCollision->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		HitBoxCollision->AttachToComponent(Weapon->WeaponStaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		HitBoxCollision->SetRelativeLocation(FVector::ZeroVector);
		HitBoxCollision->SetRelativeRotation(FRotator::ZeroRotator);

		HitBoxLifeTime(Time);
	}
}

void AHitBoxObject::HitBoxLifeTime(float Time)
{
	LifeTime = Time;

	GetWorld()->GetTimerManager().SetTimer(
		HitBoxTimerHandle,
		this,
		&AHitBoxObject::DeActiveObject_Implementation,
		LifeTime,
		false
	);
}

void AHitBoxObject::Hit(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Object"));

	Cast<AProjectileBase>(RootActor)->OnHit(OtherActor);

	HitBoxCollision->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	HitBoxCollision->SetupAttachment(Scene);
	UObjectPoolManager* Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	Pool->ReturnObject(this);
}

void AHitBoxObject::SetActive_Implementation(bool Active)
{
	bIsActive = Active;
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