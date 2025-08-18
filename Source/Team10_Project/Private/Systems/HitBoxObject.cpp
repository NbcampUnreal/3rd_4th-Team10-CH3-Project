#include "Systems/HitBoxObject.h"
#include "Components/BoxComponent.h"
#include "Weapons/Actors/ProjectileBase.h"
#include "Weapons/Actors/WeaponBase.h"
#include "Systems/ObjectPoolManager.h"
#include "Ai/Character_Monster.h"
#include "MyCharacter.h"

#include "EngineUtils.h"

AHitBoxObject::AHitBoxObject()
    : Damage(0)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	HitBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	HitBoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	HitBoxCollision->SetupAttachment(Scene);

	HitBoxCollision->SetGenerateOverlapEvents(true);
	HitBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHitBoxObject::OnOverlapHit);
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
		3.f        // 선 굵기
	);
}

void AHitBoxObject::HitBoxComp(AActor* Activator, float Height, float Width, float Vertical, float Time, bool OnlyOne)
{
    Only = OnlyOne;
	HitBoxCollision->SetBoxExtent(FVector(Width, Vertical, Height));

	AWeaponBase* Weapon  = Cast<AWeaponBase>(Activator);
	if (Weapon->GetWeaponType() == EWeaponType::Projectile)
	{
		AProjectileBase* Projectile = Cast<AProjectileBase>(Weapon);
		HitBoxCollision->SetWorldLocation(Projectile->ProjectileCollision->GetComponentLocation());
		HitBoxCollision->SetWorldRotation(Projectile->ProjectileCollision->GetComponentRotation());

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

void AHitBoxObject::OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Hit Actor!!!!!!!!!: %s"), *OtherActor->GetName());
    AObjectPoolManager* Pool = nullptr;
    for (TActorIterator<AObjectPoolManager> It(GetWorld()); It; ++It)
    {
        Pool = *It;
        break;
    }

    if (!HitObject.Contains(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Contain Actor!!!!!!!!!: %s"), *OtherActor->GetName());
        HitObject.Add(OtherActor);
    }

    AActor* ObjInstigator = this->GetInstigator();
    if (Only)
    {
        if (HitObject.Num() >= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("In Hit Object"));
            UE_LOG(LogTemp, Error, TEXT("In Hit Object %s"), *GetInstigator()->GetName());
            if (Cast<AMyCharacter>(ObjInstigator) && OtherActor->ActorHasTag("Enemy"))
            {
                Cast<ACharacter_Monster>(OtherActor)->ApplyCustomDamage(Damage);
                UE_LOG(LogTemp, Warning, TEXT("Hit Enemy from Hit Box!!!!!!!!!"));
            }
            else if (Cast<ACharacter_Monster>(ObjInstigator) && OtherActor->ActorHasTag("Player"))
            {
                Cast<AMyCharacter>(OtherActor)->GetAttributeComponent()->SetHealth(Damage);
            }
        }
    }
    else if (!Only)
    {
        for (AActor* HitObj : HitObject)
        {
            if (Cast<AMyCharacter>(ObjInstigator) && HitObj->ActorHasTag("Enemy"))
            {
                Cast<ACharacter_Monster>(HitObj)->ApplyCustomDamage(Damage);
            }
            else if (Cast<ACharacter_Monster>(ObjInstigator) && HitObj->ActorHasTag("Player"))
            {
                Cast<AMyCharacter>(HitObj)->GetAttributeComponent()->SetHealth(Damage);
            }
        }
    }

	Pool->ReturnObject(this);
}

void AHitBoxObject::SetDamage(int32 WPower)
{
    Damage = WPower;
}

int AHitBoxObject::GetDamage() const
{
    return Damage;
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

	HitBoxCollision->Activate();
	HitBoxCollision->SetNotifyRigidBodyCollision(true);
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

		HitBoxCollision->SetNotifyRigidBodyCollision(false);
		HitBoxCollision->Deactivate();

        /*Scene->SetWorldLocation(FVector::ZeroVector);
        HitBoxCollision->SetWorldLocation(FVector::ZeroVector);*/

        //SetActorLocation(FVector::ZeroVector);
        HitObject.Empty();
	}
}