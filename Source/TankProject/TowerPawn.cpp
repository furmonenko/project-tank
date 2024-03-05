#include "TowerPawn.h"

#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"

ATowerPawn::ATowerPawn()
	:Super()
{
	MaterialParameterName = "Param";
}

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
	if (Tank)
	{
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

		if (bHasLineOfSight && HitResult.GetActor() == Tank)
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
			
			if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ATurretPawn::Fire, FireRate, true);
			}
		}
		else
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
			}
		}
	}

}
