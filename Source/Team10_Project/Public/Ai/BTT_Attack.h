#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

UCLASS(Blueprintable)
class TEAM10_PROJECT_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};