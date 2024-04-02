#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AITowerController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class TANKPROJECT_API AAITowerController : public AAIController
{
	GENERATED_BODY()

	AAITowerController();

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EnemyType;
	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION(BlueprintCallable)
	void UpdateClosestEnemyAsTarget();
	
	UPROPERTY()
	TArray<AActor*> PerceivedEnemies;
};
