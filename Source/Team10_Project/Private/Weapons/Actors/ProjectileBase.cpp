#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "Systems/ObjectPoolManager.h"
#include "Systems/HitboxObject.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EngineUtils.h"

AProjectileBase::AProjectileBase()
	:bIsActive(false), ProjectileLocation(FVector::ZeroVector), ProjectileRotation(FRotator::ZeroRotator), ProjectileDir(FVector::ZeroVector), ProjectileSpeed(85000), ProjectileRange(0), Only(false)
{
	WeaponType = EWeaponType::Projectile;

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ProjectileCollision->SetupAttachment(Scene);
    ProjectileCollision->Deactivate();

	WeaponStaticMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComp->SetUpdatedComponent(ProjectileCollision);
	ProjectileMovementComp->bForceSubStepping = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComp->MaxSpeed = ProjectileSpeed;

	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	//bIsActive = true;
}

void AProjectileBase::Tick(float Time)
{
	Super::Tick(Time);

	FVector Center = ProjectileCollision->GetComponentLocation();
	FVector Extent = ProjectileCollision->GetScaledBoxExtent(); // Scale �ݿ��� ���� ũ��
	FRotator Rotation = ProjectileCollision->GetComponentRotation();
	DrawDebugBox(
		GetWorld(),
		Center,
		Extent,
		Rotation.Quaternion(),
		FColor::Green,
		true,     // ���� ����
		2.f,       // ���� �ð� (0�̸� �� �����Ӹ�)
		0,         // Depth Priority
		1.f        // �� ����
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
	UE_LOG(LogTemp, Warning, TEXT("Move"));

	ProjectileMovementComp->Velocity = ProjectileDir * ProjectileSpeed;

	ProjectileLifeTime();
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
    AObjectPoolManager* Pool = nullptr;
    for (TActorIterator<AObjectPoolManager> It(GetWorld()); It; ++It)
    {
        Pool = *It;
        break;
    }
	AHitBoxObject* HitBox = Pool->GetObject<AHitBoxObject>();
	HitBox->HitBoxComp(this, Height, Width, Vertical, LifeTime, Only);

	if (OtherActor->ActorHasTag("Enemy"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Enemy"));
		//Cast<ACharacter_Monster>(CollisionActor)->TakeDamage(TotalDamage);
	}

	Pool->ReturnObject(this);
}

void AProjectileBase::SetDamage(int32 WPower)
{
	TotalDamage = Power + WPower;
}

void AProjectileBase::ProjectileLifeTime()
{
	//LifeTime = ProjectileRange / ProjectileSpeed;

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
	this->SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
    
	ProjectileCollision->Activate();
	ProjectileCollision->SetNotifyRigidBodyCollision(true);

	ProjectileMovementComp->Activate();
}

void AProjectileBase::DeActiveObject_Implementation()
{
	if (!bIsActive) return;

	if (bIsActive)
	{
		UE_LOG(LogTemp, Warning, (TEXT("RETURN")));
		bIsActive = false;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		ProjectileCollision->SetNotifyRigidBodyCollision(false);
		ProjectileCollision->Deactivate();
		ProjectileMovementComp->Deactivate();
	}
}

