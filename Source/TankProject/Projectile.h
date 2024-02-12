#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* DefaultComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* HitBox;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Speed = 2000.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Damage")
	float Damage = 100.f;
};
