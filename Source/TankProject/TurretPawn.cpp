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

void ATurretPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (IsValid(TurretMesh))
	{
		SetTeamColorToMesh(TurretMesh);
	}
	
	if (IsValid(BaseMesh))
	{
		SetTeamColorToMesh(BaseMesh);
	}
}

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

void ATurretPawn::SetTeamColorToMesh(UStaticMeshComponent* MeshToColor)
{
	int32 materialIndex = MeshToColor->GetMaterialIndex(SlotToColor);
	UMaterialInstanceDynamic *M_TeamSlot = UMaterialInstanceDynamic::Create(MeshToColor->GetMaterial(materialIndex), this);

	if (IsValid(M_TeamSlot))
	{
		M_TeamSlot->SetVectorParameterValue(MaterialParameterName,TeamColor);
		MeshToColor->SetMaterialByName(SlotToColor, M_TeamSlot);
	}
}
