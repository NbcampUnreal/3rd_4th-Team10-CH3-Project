#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM10_PROJECT_API UObjectMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddForwardMovementComp(AActor* Activator, USkeletalMeshComponent* SkeletalMesh, float Speed);
};
