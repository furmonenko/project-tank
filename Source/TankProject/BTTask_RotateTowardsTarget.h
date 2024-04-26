#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotateTowardsTarget.generated.h"

/**
 * Task that rotates the AI's static mesh towards a target location.
 */
UCLASS()
class TANKPROJECT_API UBTTask_RotateTowardsTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_RotateTowardsTarget();

protected:
	UPROPERTY()
	class ATowerPawn* TowerPawn;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
