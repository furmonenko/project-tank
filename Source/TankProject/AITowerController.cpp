// Fill out your copyright notice in the Description page of Project Settings.


#include "AITowerController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAITowerController::AAITowerController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BT");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BB");

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAITowerController::OnPerceptionUpdated);
}

void AAITowerController::BeginPlay()
{
	Super::BeginPlay();

	/*AIPerceptionComponent = GetPerceptionComponent();
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPerceptionComponent was not initialized correctly."));
	}*/

	if(IsValid(BehaviorTreeAsset))
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}
}

void AAITowerController::OnPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "OnPerceptionUpdated");
}
