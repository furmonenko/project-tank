#include "TurretPawn.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	Health = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	TurretRotationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TurretRotationAudioComponent"));

	RootComponent = CapsuleComponent;

	TurretRotationAudioComponent->bAutoActivate = false;
	
	TurretMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetupAttachment(GetRootComponent());
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
	TurretRotationAudioComponent->SetupAttachment(GetRootComponent());

	MaterialParameterName = "Color";
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	SetTeamColor();
}

void ATurretPawn::Tick(float delta)
{
	Super::Tick(delta);

	if (TurretTargetRotation != FRotator::ZeroRotator)
	{
		RotateTurretSmooth(delta);
	}
}

void ATurretPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetTeamColor();
}

TArray<FName> ATurretPawn::GetTurretAvailableSlotNames()
{
	if (IsValid(TurretMesh))
	{
		return TurretMesh->GetMaterialSlotNames();
	}
	return TArray<FName>{};
}

TArray<FName> ATurretPawn::GetBaseAvailableSlotNames()
{
	if (IsValid(BaseMesh))
	{
		return BaseMesh->GetMaterialSlotNames();
	}
	return TArray<FName>{};
}

void ATurretPawn::SetTeamColor()
{
	if (IsValid(M_TeamSlot))
	{
		if (IsValid(TurretMesh) && IsValid(BaseMesh))
		{
			M_TeamSlot->SetVectorParameterValue(MaterialParameterName, TeamColor);
		}
	}
	else
	{
		if (IsValid(DefaultMaterial))
		{
			M_TeamSlot = UMaterialInstanceDynamic::Create(DefaultMaterial, this);

			BaseMesh->SetMaterialByName(TurretMeshSlotToColor, M_TeamSlot);
			TurretMesh->SetMaterialByName(BaseMeshSlotToColor, M_TeamSlot);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No DEFAULT MATERIAL"));
		}
	}
}

void ATurretPawn::Fire()
{
	if (ProjectileSpawnPoint)
	{
		if (SmokeEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(SmokeEffect, ProjectileSpawnPoint);
		}
		if (ShotSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotSound, ProjectileSpawnPoint->GetComponentLocation());
		}

		if (GetWorld())
		{
			FVector ProjetileLocation = ProjectileSpawnPoint->GetComponentLocation();
			FRotator ProjetileRotation = ProjectileSpawnPoint->GetComponentRotation();
			ProjetileRotation.Yaw += 90.f;

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = GetInstigator();
			SpawnParameters.Owner = this;

			if (ProjectileClass != nullptr)
			{
				AProjectile* SpawnedActor = GetWorld()->SpawnActor<AProjectile>(
					ProjectileClass, ProjetileLocation, ProjetileRotation, SpawnParameters);
			}
		}
	}
}

void ATurretPawn::RotateTurretSmooth(const float delta)
{
	FRotator CurrentRotation;
	FRotator NewRotation;

	if (IsValid(TurretMesh))
	{
		CurrentRotation = TurretMesh->GetComponentRotation();
		FRotator DeltaRotation = (TurretTargetRotation - CurrentRotation).GetNormalized();
		
		if (!DeltaRotation.IsNearlyZero(0.5f))
		{
			if (!TurretRotationAudioComponent->IsPlaying())
			{
				TurretRotationAudioComponent->Play();
			}
			
			NewRotation = FMath::RInterpTo(CurrentRotation, TurretTargetRotation, delta, RotationRate);
			TurretMesh->SetWorldRotation(NewRotation);
		}
		else
		{
			if (TurretRotationAudioComponent->IsPlaying())
			{
				TurretRotationAudioComponent->Stop();
			}
		}
	}
}

void ATurretPawn::ServerRotateTurret_Implementation(float delta)
{
	RotateTurretSmooth(delta);
}

void ATurretPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATurretPawn, TurretTargetRotation);
}
