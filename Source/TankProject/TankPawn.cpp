#include "TankPawn.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "TankPlayerController.h"
#include "Components/CapsuleComponent.h"
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

	PlayerController = Cast<APlayerController>(GetController());
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TurretTargetRotation != FRotator::ZeroRotator)
	{
		RotateTurretSmooth(DeltaSeconds);
	}

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

void ATankPawn::TurretInit()
{
	Super::TurretInit();

	OnHealthChanged(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
	OnRep_Ammo();
}

void ATankPawn::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	Super::OnHealthChanged(CurrentHealth, MaxHealth);

	PlayerController = Cast<APlayerController>(GetController());

	if (!IsValid(PlayerController))
	{
		return;
	}

	ATankPlayerController* TankPlayerController = Cast<ATankPlayerController>(PlayerController);

	if (IsValid(TankPlayerController))
	{
		TankPlayerController->UpdateHUDHealth(CurrentHealth, MaxHealth);
	}
}

void ATankPawn::OnRep_Ammo()
{
	PlayerController = Cast<APlayerController>(GetController());

	if (!IsValid(PlayerController))
	{
		return;
	}

	ATankPlayerController* TankPlayerController = Cast<ATankPlayerController>(PlayerController);

	if (IsValid(TankPlayerController))
	{
		TankPlayerController->UpdateHUDAmmoCount(AmmoCount);
	}
}

void ATankPawn::ClientCorrectPosition_Implementation(FVector CorrectPosition)
{
	FVector StartPosition = GetActorLocation();
	
	StartTime = GetWorld()->GetTimeSeconds();
	float LerpDuration = 0.1f;
	
	GetWorld()->GetTimerManager().SetTimer(InterpolationTimerHandle, [this, StartPosition, CorrectPosition, LerpDuration]()
	{
		float Alpha = (GetWorld()->GetTimeSeconds() - StartTime) / LerpDuration;
		if (Alpha < 1.0f)
		{
			SetActorLocation(FMath::Lerp(StartPosition, CorrectPosition, FMath::Clamp(Alpha, 0.0f, 1.0f)));
		}
		else
		{
			SetActorLocation(CorrectPosition);
			GetWorld()->GetTimerManager().ClearTimer(InterpolationTimerHandle);
		}
	}, 0.01, true);
}

void ATankPawn::ClientCorrectRotation_Implementation(FRotator CorrectRotation)
{
	FRotator StartRotation = GetActorRotation();
	float LerpDuration = 0.1f;

	GetWorld()->GetTimerManager().SetTimer(InterpolationTimerHandle, [this, StartRotation, CorrectRotation, LerpDuration]()
	{
		float Alpha = (GetWorld()->GetTimeSeconds() - StartTime) / LerpDuration;
		if (Alpha < 1.0f)
		{
			SetActorRotation(FMath::Lerp(StartRotation, CorrectRotation, FMath::Clamp(Alpha, 0.0f, 1.0f)));
		}
		else
		{
			SetActorRotation(CorrectRotation);
			GetWorld()->GetTimerManager().ClearTimer(InterpolationTimerHandle);
		}
	}, 0.01, true);

	StartTime = GetWorld()->GetTimeSeconds();
}

void ATankPawn::HandleInputMove(float ActionValue)
{
	Move(ActionValue);

	FInputState NewState;
	NewState.MoveValue = ActionValue;
	NewState.Timestamp = GetWorld()->GetTimeSeconds();
	NewState.Position = GetActorLocation();
	NewState.Rotation = GetActorRotation();
	InputBuffer.Add(NewState);

	if (!HasAuthority())
	{
		ServerMove(ActionValue);
	}
}

void ATankPawn::ServerMove_Implementation(float ActionValue)
{
	Move(ActionValue);

	if (FVector::DistSquared(GetActorLocation(), ServerPosition) > 500) // Threshold value
	{
		ServerPosition = GetActorLocation();
		ClientCorrectPosition(ServerPosition);
	}
}

void ATankPawn::Move(float ActionValue)
{
	MovementSpeed = FMath::FInterpTo(MovementSpeed, MaxSpeed * ActionValue, GetWorld()->GetDeltaSeconds(),
									 AccelerationRate);

	if (FMath::Sign(MovementSpeed) != FMath::Sign(ActionValue) && ActionValue != 0)
	{
		MovementSpeed = 0.f;
		return;
	}

	if (IsValid(GetWorld()) && MovementSpeed != 0.f)
	{
		FVector MovementVector = FVector(MovementSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
		AddActorLocalOffset(MovementVector, true);
	}
}

void ATankPawn::HandleInputTurn(float ActionValue)
{
	Turn(ActionValue);

	FInputState NewState;
	NewState.TurnValue = ActionValue;
	NewState.Timestamp = GetWorld()->GetTimeSeconds();
	NewState.Position = GetActorLocation();
	NewState.Rotation = GetActorRotation();
	InputBuffer.Add(NewState);

	if (!HasAuthority())
	{
		ServerTurn(ActionValue);
	}
}

void ATankPawn::ServerTurn_Implementation(float ActionValue)
{
	Turn(ActionValue);

	FRotator CurrentRotation = GetActorRotation().GetNormalized();
	FRotator ServerStoredRotation = ServerRotation.GetNormalized();

	if (!CurrentRotation.Equals(ServerStoredRotation, 2.0f)) // Test value
	{
		ClientCorrectRotation(GetActorRotation());
		ServerRotation = GetActorRotation();
	}
}

void ATankPawn::Turn(float ActionValue)
{
	if (IsValid(GetWorld()) && bCanMove)
	{
		FRotator TurnRate = FRotator(0.f, ActionValue * TurningSpeed * GetWorld()->GetDeltaSeconds(), 0.f);
		AddActorLocalRotation(TurnRate, true);
	}
}

void ATankPawn::PlayLocalCameraShake()
{
	if (IsLocallyControlled() && CameraShakeClass)
	{
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(CameraShakeClass);
		}
	}
}

void ATankPawn::ClientPlayFireEffects_Implementation()
{
	if (IsLocallyControlled())
	{
		PlayFireEffects();
		PlayLocalCameraShake();
	}
}

void ATankPawn::Die()
{
	TankDied.Broadcast();
	/*if (DeathSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	}*/
}

void ATankPawn::SetTargetLookRotation()
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

void ATankPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATankPawn, InputBuffer);
	DOREPLIFETIME(ATankPawn, MovementSpeed);
	DOREPLIFETIME(ATankPawn, TankLocation);
	DOREPLIFETIME(ATankPawn, TankRotation);
	DOREPLIFETIME(ATankPawn, AmmoCount);
}
