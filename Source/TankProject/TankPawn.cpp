#include "TankPawn.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"



ATankPawn::ATankPawn()
	: Super()
{
	MovementSmokeParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticles"));
	MovementSmokeParticleSystemComponent->SetupAttachment(GetRootComponent());
	MovementAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementAudioComponent"));

	MovementAudioComponent->bAutoActivate = false;
	MovementSmokeParticleSystemComponent->bAutoActivate = false;

	MovementAudioComponent->SetupAttachment(GetRootComponent());
	
	if (IsValid(CapsuleComponent))
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MovementSmokeParticleSystemComponent && MovementAudioComponent)
	{
		if (isMoving)
		{
			if (!MovementAudioComponent->IsPlaying())
			{
				MovementAudioComponent->Play();
			}
			MovementSmokeParticleSystemComponent->Activate(true);
		}
		else
		{
			MovementAudioComponent->Stop();
			MovementSmokeParticleSystemComponent->Deactivate();
		}
	}

}

void ATankPawn::Move(float ActionValue)
{
	MovementSpeed = FMath::FInterpTo(MovementSpeed, MaxSpeed * ActionValue, GetWorld()->GetDeltaSeconds(),
	                                 AccelerationRate);

	if (IsValid(GetWorld()) && MovementSpeed != 0.f)
	{
		const FVector MovementVector = FVector(MovementSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
		AddActorLocalOffset(MovementVector, true);
	}
}

void ATankPawn::Turn(float ActionValue)
{
	if (IsValid(GetWorld()))
	{
		const FRotator TargetRotation = FRotator(0.f, ActionValue * TurningSpeed * GetWorld()->GetDeltaSeconds(), 0.f);
		AddActorLocalRotation(TargetRotation, true);
	}
}

void ATankPawn::Die()
{
	if (DeathSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	}
}

void ATankPawn::SetTargetLookRotation(FRotator Rotation)
{
	if (IsValid(PlayerController) && IsValid(TurretMesh))
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		const FVector Direction = HitResult.ImpactPoint - TurretMesh->GetComponentLocation();

		DrawDebugCrosshairs(GetWorld(), HitResult.Location, FRotator::ZeroRotator, 40.f, FColor::Red, false, 0.05f);
		TurretTargetRotation = Direction.Rotation();

		// By default tower is rotated 90 degrees wrong. Fixing it here.
		TurretTargetRotation.Yaw -= 90.f;
		TurretTargetRotation.Pitch = 0.f;
	}
}
