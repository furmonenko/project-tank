#include "TankPawn.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "TankPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


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

void ATankPawn::OnRep_TankLocation()
{
	SetActorLocation(TankLocation);
}

void ATankPawn::ServerMove_Implementation(float ActionValue)
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
		
		TankLocation = GetActorLocation();
	}
}

void ATankPawn::OnRep_TankRotation()
{
	SetActorRotation(TankRotation);
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

void ATankPawn::ServerTurn_Implementation(float ActionValue)
{
	if (IsValid(GetWorld()) && bCanMove)
	{
		FRotator TurnRate = FRotator(0.f, ActionValue * TurningSpeed * GetWorld()->GetDeltaSeconds(), 0.f);

		AddActorLocalRotation(TurnRate, true);
		TankRotation = GetActorRotation();
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
	DOREPLIFETIME(ATankPawn, TankLocation);
	DOREPLIFETIME(ATankPawn, TankRotation);
	DOREPLIFETIME(ATankPawn, AmmoCount);
}
