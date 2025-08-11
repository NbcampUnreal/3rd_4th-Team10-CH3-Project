#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CustomMoveTo_CPP.generated.h"

UCLASS()
class TEAM10_PROJECT_API UBTT_CustomMoveTo_CPP : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_CustomMoveTo_CPP();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};