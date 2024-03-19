#include "TankPawn.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"


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
	
	SetReplicates(true);
	SetReplicateMovement(true);

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
		FVector MovementVector = FVector(MovementSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
		AddActorLocalOffset(MovementVector, true);
		
		if (!HasAuthority())
		{
			ServerMove(ActionValue);
		}
	}
}

void ATankPawn::ServerMove_Implementation(float ActionValue)
{
	Move(ActionValue);
}

void ATankPawn::Turn(float ActionValue)
{
	if (IsValid(GetWorld()))
	{
		const FRotator TurnRate = FRotator(0.f, ActionValue * TurningSpeed * GetWorld()->GetDeltaSeconds(), 0.f);
		
		AddActorLocalRotation(TurnRate, true);

		if (!HasAuthority())
		{
			ServerTurn(ActionValue);
		}
	}
}

void ATankPawn::ServerTurn_Implementation(float ActionValue)
{
	Turn(ActionValue);
}
bool ATankPawn::ServerTurn_Validate(float ActionValue)
{
	return true;
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
		FRotator NewRotation = Direction.Rotation();

		// By default tower is rotated 90 degrees wrong. Fixing it here.
		NewRotation.Yaw -= 90.f;
		NewRotation.Pitch = 0.f;

		if (!HasAuthority())
		{
			ServerSetTargetLookRotation(NewRotation);
		}
		else
		{
			TurretTargetRotation = NewRotation;
		}
	}
}

void ATankPawn::ServerSetTargetLookRotation_Implementation(FRotator NewRotation)
{
	TurretTargetRotation = NewRotation;
}

void ATankPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATankPawn, MovementSpeed);
	DOREPLIFETIME(ATankPawn, TurningSpeed);
	DOREPLIFETIME(ATankPawn, MaxSpeed);
	DOREPLIFETIME(ATankPawn, AccelerationRate);
}
