#include "TurretPawn.h"
#include "Components/CapsuleComponent.h"

ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	
	RootComponent = CapsuleComponent;
	
	TurretMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetupAttachment(GetRootComponent());
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

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
	RotateTurretSmooth();
}

void ATurretPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetTeamColor();
}

TArray<FName> ATurretPawn::GetTurretAvailableSlotNames()
{
	if(IsValid(TurretMesh))
	{
		return TurretMesh->GetMaterialSlotNames();
	}
	return TArray<FName>{};
}

TArray<FName> ATurretPawn::GetBaseAvailableSlotNames()
{
	if(IsValid(BaseMesh))
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
		DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 10.f, 30, FColor::Red, true);

		if (GetWorld())
		{
			FVector ProjetileLocation = ProjectileSpawnPoint->GetComponentLocation();
			FRotator ProjetileRotation = ProjectileSpawnPoint->GetComponentRotation();
			ProjetileRotation.Yaw += 90.f;

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this; 
			
			if (ProjectileClass != nullptr)
			{
				AProjectile* SpawnedActor = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjetileLocation, ProjetileRotation);
			}
		}
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "Fire");
	}
}

void ATurretPawn::RotateTurretSmooth() const 
{
	if(IsValid(TurretMesh))
	{
		const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
		
		const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TurretTargetRotation, GetWorld()->GetDeltaSeconds(), RotationRate);
		TurretMesh->SetWorldRotation(NewRotation);
		
#if WITH_EDITOR
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, NewRotation.ToString());
			}
		}
#endif
}
