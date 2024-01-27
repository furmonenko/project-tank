#include "TurretPawn.h"
#include "Components/CapsuleComponent.h"

ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	TurningSpeed = 50.f;
	
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

void ATurretPawn::RotateTurretSmooth() const 
{
	if(IsValid(TurretMesh))
	{
		const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
		const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TurretTargetRotation, GetWorld()->GetDeltaSeconds(), 2.f);
		TurretMesh->SetWorldRotation(NewRotation);
#if WITH_EDITOR
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, NewRotation.ToString());
			}
		}
#endif
}
