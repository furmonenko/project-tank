#include "TowerPawn.h"

#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"

ATowerPawn::ATowerPawn()
	: Super()
{
	MaterialParameterName = "Param";
}

void ATowerPawn::SetTargetLookRotation(const FVector& TargetLocation)
{
	Super::SetTargetLookRotation();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	const FVector StartLocation = GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	LookAtRotation.Yaw -= 90.f;

	if (IsValid(ProjectileSpawnPoint))
	{
		FRotator NewRotation = ProjectileSpawnPoint->GetComponentRotation();
		NewRotation.Pitch = LookAtRotation.Pitch; 
		ProjectileSpawnPoint->SetWorldRotation(NewRotation);
	}

	LookAtRotation.Pitch = 0.f;
	TurretTargetRotation = LookAtRotation;	
}

