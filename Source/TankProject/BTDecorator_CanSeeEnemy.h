#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CanSeeEnemy.generated.h"

UCLASS()
class TANKPROJECT_API UBTDecorator_CanSeeEnemy : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_CanSeeEnemy();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
