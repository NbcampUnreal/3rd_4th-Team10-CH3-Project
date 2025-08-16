#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h" 
#include "AIController_Monster.generated.h"

                                   
UCLASS()
class TEAM10_PROJECT_API AAIController_Monster : public ADetourCrowdAIController
{
    GENERATED_BODY()

public:
    AAIController_Monster();

protected:
    // ���Ϳ� ������ �� ȣ��� �Լ� (����)
    virtual void OnPossess(APawn* InPawn) override;

private:
    // �����̺�� Ʈ���� ������ ������Ʈ
    UPROPERTY()
    TObjectPtr<class UBehaviorTreeComponent> BTComponent;
    UPROPERTY()
    TObjectPtr<class UBlackboardComponent> BBComponent;

    // �þ�(Perception) ������Ʈ
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

    // �����Ϳ��� ������ �����̺�� Ʈ�� ����
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

    // AI�� ���� ���� �����Ǿ��� �� ȣ��� �Լ� (����)
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};