#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/RangeWeapon.h"
#include "Systems/ObjectPoolManager.h"
#include "Systems/HitboxObject.h"
#include "AI/Character_Monster.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EngineUtils.h"

AProjectileBase::AProjectileBase()
	:bIsActive(false), ProjectileLocation(FVector::ZeroVector), ProjectileRotation(FRotator::ZeroRotator), ProjectileDir(FVector::ZeroVector), 
    ProjectileSpeed(100), ProjectileRange(0), Only(false)
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
    ProjectileMovementComp->bAutoActivate = false;
	ProjectileMovementComp->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComp->MaxSpeed = ProjectileSpeed;

	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	//bIsActive = true;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
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
		1.f        // 선 굵기
	);
}

void AProjectileBase::Activate(ARangeWeapon* ActiveWeapon, FVector ProjectileLoc, FRotator ProjectileRotate, FVector FireDir)
{
	UE_LOG(LogTemp, Warning, TEXT("Active"));
    ProjectileCollision->SetRelativeLocation(FVector::ZeroVector);
    WeaponStaticMesh->SetRelativeLocation(FVector::ZeroVector);

    ProjectileLocation = ProjectileLoc;
	ProjectileRotation = ProjectileRotate;
	ProjectileDir = FireDir;
    SetActorLocationAndRotation(ProjectileLocation, ProjectileRotation, false, nullptr, ETeleportType::TeleportPhysics);
    ProjectileCollision->SetWorldLocationAndRotation(ProjectileLocation, ProjectileRotation);

	SetDamage(ActiveWeapon->GetPower());

	ProjectileMovement();
}

void AProjectileBase::ProjectileMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));

	//ProjectileMovementComp->Velocity = ProjectileDir * ProjectileSpeed;
    ProjectileCollision->SetPhysicsLinearVelocity(ProjectileDir * ProjectileSpeed);
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
    SetHitScale();
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
		Cast<ACharacter_Monster>(OtherActor)->TakeDamage(TotalDamage);
	}

	Pool->ReturnObject(this);
}

void AProjectileBase::SetDamage(int32 WPower)
{
	TotalDamage = Power + WPower;
}

void AProjectileBase::ProjectileLifeTime()
{
	LifeTime = ProjectileRange / ProjectileSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		ProjectileTimerHandle,
		this,
		&AProjectileBase::DeActiveObject_Implementation,
		LifeTime,
		false
	);
}

FVector AProjectileBase::SetHitScale()
{
    BoxExtent = ProjectileCollision->GetScaledBoxExtent();
    CollisionSize = BoxExtent;
    Height = CollisionSize.Z;
    Width = CollisionSize.X;
    Vertical = CollisionSize.Y;

    FVector HitScale = { Height, Width, Vertical };
    return HitScale;
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
	ProjectileCollision->Activate();
	ProjectileCollision->SetNotifyRigidBodyCollision(true);
    //ProjectileCollision->SetSimulatePhysics(true);

    ProjectileMovementComp->SetUpdatedComponent(ProjectileCollision);
	ProjectileMovementComp->Activate();
    ProjectileMovementComp->SetComponentTickEnabled(true);
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

		ProjectileCollision->SetNotifyRigidBodyCollision(false);
		ProjectileCollision->Deactivate();
        //ProjectileCollision->SetSimulatePhysics(false);

        ProjectileMovementComp->Deactivate();
        ProjectileMovementComp->SetComponentTickEnabled(false);

        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        Scene->SetRelativeLocation(FVector::ZeroVector);
        ProjectileCollision->SetRelativeLocation(FVector::ZeroVector);
        WeaponStaticMesh->SetRelativeLocation(FVector::ZeroVector);
		
        SetActorLocation(FVector::ZeroVector);
	}
}

