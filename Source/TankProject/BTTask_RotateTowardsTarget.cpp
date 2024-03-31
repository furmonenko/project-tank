#include "BTTask_RotateTowardsTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "TowerPawn.h"
#include "GameFramework/Actor.h"

UBTTask_RotateTowardsTarget::UBTTask_RotateTowardsTarget()
{
	NodeName = "Rotate Towards Target";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RotateTowardsTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = AIController->GetPawn();
	if (!IsValid(AIController))
	{
		return EBTNodeResult::Failed;
	}

	TowerPawn = Cast<ATowerPawn>(OwnerPawn);
	if (!IsValid(TowerPawn))
	{
		return EBTNodeResult::Failed;
	}

	if (!GetWorld())
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateTowardsTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsValid(TowerPawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	if (TowerPawn->RotateTurretSmooth(DeltaSeconds))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}