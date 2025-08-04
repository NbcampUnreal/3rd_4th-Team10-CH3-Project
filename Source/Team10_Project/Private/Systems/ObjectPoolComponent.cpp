#include "Systems/ObjectPoolComponent.h"

UObjectPoolComponent::UObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

}
