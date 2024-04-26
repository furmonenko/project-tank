#include "BTTask_SetRandomPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_SetRandomPoint::UBTTask_SetRandomPoint()
{
	NodeName = "Set Random Location";
}

EBTNodeResult::Type UBTTask_SetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = Pawn->GetActorLocation();
	FVector RandomDirection = FMath::VRand() * SearchRadius;
	FVector RandomLocation = Origin + RandomDirection;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation);

	return EBTNodeResult::Succeeded;
}
