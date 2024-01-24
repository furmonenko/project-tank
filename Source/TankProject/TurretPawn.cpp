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
			SetTeamColorToMesh(TurretMesh);
		}
	
		if (IsValid(BaseMesh))
		{
			SetTeamColorToMesh(BaseMesh);
		}
	}
	else
	{
		int32 MaterialIndex = BaseMesh->GetMaterialIndex(SlotToColor);
		M_TeamSlot = UMaterialInstanceDynamic::Create(BaseMesh->GetMaterial(MaterialIndex), this);
		BaseMesh->SetMaterialByName(SlotToColor, M_TeamSlot);
		TurretMesh->SetMaterialByName(SlotToColor, M_TeamSlot);
	}
}
#endif

TArray<FName> ATurretPawn::GetAvailableSlotNames()
{
	if (IsValid(TurretMesh))
	{
		return {"Team_Material"};
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
		M_TeamSlot->SetVectorParameterValue(MaterialParameterName,TeamColor);
	}
}
