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
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());

	MaterialParameterName = "Color";
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();	
}

#if WITH_EDITOR
void ATurretPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (IsValid(M_TeamSlot))
	{
		if (IsValid(TurretMesh))
		{
			SetTeamColor();
		}
	
		if (IsValid(BaseMesh))
		{
			SetTeamColor();
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
#endif

TArray<FName> ATurretPawn::GetTurretAvailableSlotNames()
{
	if(IsValid(TurretMesh))
	{
		return TurretMesh->GetMaterialSlotNames();
	}
	else
	{
		return TArray<FName>{};
	}
}

TArray<FName> ATurretPawn::GetBaseAvailableSlotNames()
{
	if(IsValid(BaseMesh))
	{
		return BaseMesh->GetMaterialSlotNames();
	}
	else
	{
		return TArray<FName>{};
	}
}

void ATurretPawn::SetTeamColor()
{
	if (IsValid(M_TeamSlot))
	{
		M_TeamSlot->SetVectorParameterValue(MaterialParameterName, TeamColor);
	}
}
