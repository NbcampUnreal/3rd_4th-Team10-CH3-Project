#include "Ai/AIController_Monster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Ai/Character_Monster.h"

AAIController_Monster::AAIController_Monster()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// �þ� ���� ��ü ����
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		// �þ� �ݰ� �� �� ���� ����
		SightConfig->SightRadius = 100000.0f;
		SightConfig->LoseSightRadius = 15000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;

		// --- [�߰��� �κ�] ---
		// ������ ����� ����ϴ� �ð��� �����մϴ�. 0���� �����ϸ� ������ ����մϴ�.
		SightConfig->SetMaxAge(0.0f);
		// --------------------

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// ������ �þ� ������ Perception ������Ʈ�� ����
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}

// OnPossess �Լ��� ���� ���� ����
void AAIController_Monster::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		BBComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BTComponent->StartTree(*BehaviorTreeAsset);
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Monster::OnTargetPerceptionUpdated);
}

// OnTargetPerceptionUpdated �Լ��� ���� ���� ����
void AAIController_Monster::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
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