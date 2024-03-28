// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanSeeEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "WorldCollision.h"
#include "Engine/World.h"

UBTDecorator_CanSeeEnemy::UBTDecorator_CanSeeEnemy()
{
	NodeName = "Can See Enemy?";
}

bool UBTDecorator_CanSeeEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		return false;
	}

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!IsValid(BlackboardComp))
	{
		return false;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(TargetActor))
	{
		return false;
	}

	FVector AILocation = AIController->GetPawn()->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	FHitResult HitResult;
	
	return AIController->LineOfSightTo(TargetActor);
}