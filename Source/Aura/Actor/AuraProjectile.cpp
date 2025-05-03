// Copyright Diegothic


#include "AuraProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Collision/AuraCollisionChannels.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.0f;
	ProjectileMovement->MaxSpeed = 550.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);

	if (IsValid(SpawnEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SpawnEffect, GetActorLocation());
	}

	if (IsValid(LoopingSound))
	{
		UGameplayStatics::SpawnSoundAttached(
			LoopingSound,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&ThisClass::OnLifeEnded,
		LifeSpan,
		false
	);
}

void AAuraProjectile::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (!bHit)
	{
		PlayOnHitEffects(SweepResult);
		bHit = true;
	}

	if (HasAuthority())
	{
		MulticastOnHit(SweepResult);
		Destroy();
	}
}

void AAuraProjectile::PlayOnHitEffects(const FHitResult& HitResult) const
{
	if (IsValid(ImpactSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitResult.ImpactPoint);
	}

	if (IsValid(ImpactEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactEffect,
			HitResult.ImpactPoint
		);
	}
}

void AAuraProjectile::MulticastOnHit_Implementation(const FHitResult& HitResult)
{
	if (!bHit)
	{
		PlayOnHitEffects(HitResult);
		bHit = true;
	}
}

void AAuraProjectile::OnLifeEnded()
{
	if (!bHit)
	{
		PlayOnLifeEndedEffects();
		bHit = true;
	}

	if (HasAuthority())
	{
		MulticastOnLifeEnded();
		Destroy();
	}
}

void AAuraProjectile::MulticastOnLifeEnded_Implementation()
{
	if (!bHit)
	{
		PlayOnLifeEndedEffects();
		bHit = true;
	}
}

void AAuraProjectile::PlayOnLifeEndedEffects() const
{
	if (IsValid(LifeEndedSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, LifeEndedSound, GetActorLocation());
	}

	if (IsValid(LifeEndedEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			LifeEndedEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}
