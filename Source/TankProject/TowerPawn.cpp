#include "TowerPawn.h"

#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"

ATowerPawn::ATowerPawn()
	: Super()
{
	MaterialParameterName = "Param";
}

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Tank)
	{
		return;
	}

	const FVector StartLocation = GetActorLocation();
	const FVector TargetLocation = Tank->GetActorLocation();
	const FVector LineStartLocation = ProjectileSpawnPoint->GetComponentLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHasLineOfSight = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LineStartLocation,
		TargetLocation,
		ECC_Visibility,
		CollisionParams
	);

	UWorld* World = GetWorld();
	
	if (bHasLineOfSight && HitResult.GetActor() == Tank && IsValid(World))
	{
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
		
		if (!World->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			World->GetTimerManager().SetTimer(FireTimerHandle, this, &ATurretPawn::ServerFire, FireRate, true);
		}
	}
	else
	{
		if (World->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			World->GetTimerManager().ClearTimer(FireTimerHandle);
		}
	}
}
