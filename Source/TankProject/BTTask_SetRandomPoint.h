#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTTask_SetRandomPoint.generated.h"

/**
 * Sets TargetLocation to a random Vector within a specified radius.
 */
UCLASS()
class TANKPROJECT_API UBTTask_SetRandomPoint : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetRandomPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
	UPROPERTY(EditAnywhere)
	float SearchRadius = 1000.0f;
};
