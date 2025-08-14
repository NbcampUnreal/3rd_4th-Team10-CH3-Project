#include "Ai/AIController_Monster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyCharacter.h" // <--- [����] �÷��̾� Ŭ������ �˾ƾ� �ϹǷ� �� ����� �߰��մϴ�.

AAIController_Monster::AAIController_Monster()
{
    BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
    BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 100000.0f;
        SightConfig->LoseSightRadius = 15000.0f;
        SightConfig->PeripheralVisionAngleDegrees = 360.0f;
        SightConfig->SetMaxAge(0.0f);

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

        AIPerceptionComponent->ConfigureSense(*SightConfig);
        AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    }
}

void AAIController_Monster::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        BBComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
        RunBehaviorTree(BehaviorTreeAsset);
    }

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Monster::OnTargetPerceptionUpdated);
}

// --- [������ �κ�] ---
// OnTargetPerceptionUpdated �Լ��� ������ �����մϴ�.
void AAIController_Monster::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // 1. ������ ���Ͱ� �÷��̾� ĳ�������� Ȯ���մϴ�.
    AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(Actor);

    // 2. ���� �÷��̾� ĳ���Ͱ� �ƴ϶�� (�ٸ� �����̰ų� �ٸ� ��ü���), �ƹ��͵� ���� �ʰ� �����մϴ�.
    if (!PlayerCharacter)
    {
        return;
    }

    // 3. ���� �÷��̾� ĳ������ ����, �����忡 TargetActor�� �����ϰų� ����ϴ�.
    if (Stimulus.WasSuccessfullySensed())
    {
        // ���������� �ôٸ� �������� TargetActor�� ����
        BBComponent->SetValueAsObject(FName("TargetActor"), Actor);
    }
    else
    {
        // ���ƴٸ� �������� TargetActor�� ���
        BBComponent->ClearValue(FName("TargetActor"));
    }
}