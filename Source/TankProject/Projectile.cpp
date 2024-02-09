#include "Projectile.h"
#include "Components/BoxComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultComponent = CreateDefaultSubobject<USceneComponent>("DefaultComponent");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	
	RootComponent = DefaultComponent;
	HitBox->SetupAttachment(GetRootComponent());
	StaticMesh->SetupAttachment(GetRootComponent());

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ForwardVector = GetActorForwardVector();
	FVector Velocity = ForwardVector * Speed * DeltaTime;
	AddActorWorldOffset(Velocity);
}

