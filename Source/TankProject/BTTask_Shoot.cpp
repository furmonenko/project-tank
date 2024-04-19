#include "BTTask_Shoot.h"
#include "AIController.h"
#include "TurretPawn.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

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

	ATurretPawn* TurretPawn = Cast<ATurretPawn>(OwnerPawn);
	if (!IsValid(TurretPawn))
	{
		return EBTNodeResult::Failed;
	}

	TurretPawn->HandleFireInput();
	return EBTNodeResult::Succeeded;
}
