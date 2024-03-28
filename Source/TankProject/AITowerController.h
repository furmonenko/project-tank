// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AITowerController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class TANKPROJECT_API AAITowerController : public AAIController
{
	GENERATED_BODY()

	AAITowerController();

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& DetectedActors);
};
