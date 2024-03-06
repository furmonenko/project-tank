#include "Projectile.h"

#include "TurretPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::HitTarget);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ForwardVector = GetActorForwardVector();
	FVector Velocity = ForwardVector * Speed * DeltaTime;
	AddActorWorldOffset(Velocity);
}

void AProjectile::HitTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATurretPawn* TurretPawn = Cast<ATurretPawn>(OtherActor);

	if (IsValid(TurretPawn))
	{
		UGameplayStatics::ApplyDamage(TurretPawn, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
	
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
		
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}
	Destroy();
}

