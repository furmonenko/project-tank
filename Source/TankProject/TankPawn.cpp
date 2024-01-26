#include "TankPawn.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"


ATankPawn::ATankPawn()
	:Super()
{
	PlayerController = nullptr;
	MovementSpeed = 0.f;
	MaxSpeed = 200.f;
	AccelerationRate = 5.f;
	DecelerationRate = 5.f;

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (TurretTargetRotation != FRotator::ZeroRotator)
	{
		RotateTurretSmooth();
	}

}

void ATankPawn::Move(float ActionValue)
{
	MovementSpeed = FMath::FInterpTo(MovementSpeed, MaxSpeed * ActionValue, GetWorld()->GetDeltaSeconds(), AccelerationRate);
	
	if (IsValid(GetWorld()) && MovementSpeed != 0.f)
	{
		const FVector MovementVector = FVector(MovementSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
		this->AddActorLocalOffset(MovementVector, true);
	}
}

void ATankPawn::Turn(float ActionValue)
{
	if(IsValid(GetWorld()))
	{
		FRotator TargetRotation = FRotator(0.f, ActionValue * 50.f * GetWorld()->GetDeltaSeconds(), 0.f);
		this->AddActorLocalRotation(TargetRotation, true);
	}
}

void ATankPawn::Look(FVector2D ActionValue)
{
	if (PlayerController)
	{
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, "Player Controller");

		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		const FVector Direction = HitResult.ImpactPoint - TurretMesh->GetComponentLocation();
		
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f, 30, FColor::Blue);
		TurretTargetRotation = Direction.Rotation();
		
		// Костильчик :( Без цього башня рівно на 90 градусів дивиться в інший бік :(
		TurretTargetRotation.Yaw -= 90.f;
		TurretTargetRotation.Pitch = 0.f;
	}
}

void ATankPawn::Fire()
{
	if (ProjectileSpawnPoint)
	{
		DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 10.f, 30, FColor::Red, true);
	}
}



