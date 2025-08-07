#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "Systems/ObjectPoolManager.h"
#include "Systems/HitboxObject.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
	:bIsActive(false), ProjectileSpeed(0), ProjectileRange(0), Only(false)
{
	WeaponType = EWeaponType::Projectile;

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ProjectileCollision->SetupAttachment(Scene);
	ProjectileCollision->SetNotifyRigidBodyCollision(true);

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	WeaponStaticMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComp->SetUpdatedComponent(ProjectileCollision);
	ProjectileMovementComp->bForceSubStepping = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComp->MaxSpeed = ProjectileSpeed;

	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

void AProjectileBase::Tick(float Time)
{
	Super::Tick(Time);

	FVector Center = ProjectileCollision->GetComponentLocation();
	FVector Extent = ProjectileCollision->GetScaledBoxExtent(); // Scale 반영된 실제 크기
	FRotator Rotation = ProjectileCollision->GetComponentRotation();
	DrawDebugBox(
		GetWorld(),
		Center,
		Extent,
		Rotation.Quaternion(),
		FColor::Green,
		true,     // 지속 여부
		2.f,       // 지속 시간 (0이면 한 프레임만)
		0,         // Depth Priority
		2.f        // 선 굵기
	);
}

void AProjectileBase::Activate(ARangeWeapon* ActiveWeapon, FVector ProjectileLoc, FRotator ProjectileRotate, FVector FireDir)
{
	UE_LOG(LogTemp, Warning, TEXT("Active"));
	ProjectileLocation = ProjectileLoc;
	ProjectileRotation = ProjectileRotate;
	ProjectileDir = FireDir;

	this->SetActorLocation(ProjectileLocation);
	this->SetActorRotation(ProjectileRotation);

	SetDamage(ActiveWeapon->GetPower());

	ProjectileMovement();
}

void AProjectileBase::ProjectileMovement()
{
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	UObjectPoolManager* Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	AHitBoxObject* HitBox = Pool->GetObject<AHitBoxObject>();
	HitBox->HitBoxComp(this, Height, Width, Vertical, LifeTime, Only);

	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	if (OtherActor->ActorHasTag("Enemy"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Enemy"));
		//Cast<ACharacter_Monster>(CollisionActor)->TakeDamage(TotalDamage);
	}

	//Pool->ReturnObject(this);
}

void AProjectileBase::SetDamage(int32 WPower)
{
	TotalDamage = Power + WPower;
}

void AProjectileBase::ProjectileLifeTime()
{
	LifeTime = 100;//ProjectileRange / ProjectileSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		ProjectileTimerHandle,
		this,
		&AProjectileBase::DeActiveObject_Implementation,
		LifeTime,
		false
	);
}

void AProjectileBase::SetActive_Implementation(bool Active)
{
	bIsActive = Active;
}

bool AProjectileBase::GetIsActive_Implementation()
{
	return bIsActive;
}

void AProjectileBase::ActiveObject_Implementation()
{
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void AProjectileBase::DeActiveObject_Implementation()
{
	if (!bIsActive) return;

	if (bIsActive)
	{
		bIsActive = false;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		ProjectileCollision->Deactivate();
		ProjectileMovementComp->Deactivate();
	}
}

