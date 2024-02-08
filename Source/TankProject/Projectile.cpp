#include "Projectile.h"
#include "Components/BoxComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	
	RootComponent = HitBox;
	StaticMesh->SetupAttachment(GetRootComponent());

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

