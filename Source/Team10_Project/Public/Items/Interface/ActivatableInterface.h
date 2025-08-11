#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActivatableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UActivatableInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM10_PROJECT_API IActivatableInterface
{
	GENERATED_BODY()

public:
	virtual void ActivatableItem() = 0;
};