// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

public:
	FORCEINLINE const UProjectileMovementComponent* GetProjectileMovement() const
	{
		return ProjectileMovement;
	}

private:
	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void PlayOnHitEffects(const FHitResult& HitResult) const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnHit(const FHitResult& HitResult);

	void OnLifeEnded();

	void PlayOnLifeEndedEffects() const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnLifeEnded();

private:
	UPROPERTY(VisibleAnywhere, Category = "Aura|Projectile")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> SpawnEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> LifeEndedEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> LifeEndedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Projectile", meta = (AllowPrivateAccess = "true"))
	float LifeSpan = 15.0f;

	FTimerHandle LifeTimerHandle;

	bool bHit = false;
};
