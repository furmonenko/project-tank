#include "TurretPawn.h"
#include "Components/CapsuleComponent.h"

ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	ProjectileSpawnPoint = CreateDefaultSubobject<UCapsuleComponent>("ProjectileSpawnPoint");

	RootComponent = CapsuleComponent;
	
	TurretMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetupAttachment(GetRootComponent());
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());
	
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

