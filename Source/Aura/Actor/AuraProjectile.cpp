// Copyright Diegothic


#include "AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemStatics.h"
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

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);

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

void AAuraProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);

	Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnSphereBeginOverlap);

	Super::EndPlay(EndPlayReason);
}

void AAuraProjectile::OnSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	HandleProjectileHit(OtherActor, bFromSweep ? static_cast<FVector>(SweepResult.ImpactPoint) : GetActorLocation());
}

void AAuraProjectile::HandleProjectileHit(AActor* InHitActor, const FVector& InImpactPoint)
{
	if (bHit || !HasAuthority())
	{
		return;
	}

	if (InHitActor == this)
	{
		return;
	}

	if (DamageEffectSpecHandle.IsValid())
	{
		const FGameplayEffectContextHandle& DamageEffectContext
			= DamageEffectSpecHandle.Data.Get()->GetEffectContext();
		if (const AActor* const EffectCauser = DamageEffectContext.GetEffectCauser();
			InHitActor == EffectCauser
			|| UAuraAbilitySystemStatics::AreActorsFriendly(EffectCauser, InHitActor)
		)
		{
			return;
		}
	}

	const FVector HitLocation_WS = InImpactPoint;

	bHit = true;
	PlayOnHitEffects(HitLocation_WS);

	MulticastOnHit(HitLocation_WS);

	if (UAbilitySystemComponent* const TargetASC
		= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InHitActor)
	)
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
	}

	Destroy();
}

void AAuraProjectile::PlayOnHitEffects(const FVector& InLocation_WS) const
{
	if (IsValid(ImpactSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, InLocation_WS);
	}

	if (IsValid(ImpactEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactEffect,
			InLocation_WS
		);
	}
}

void AAuraProjectile::MulticastOnHit_Implementation(const FVector& InLocation_WS)
{
	if (bHit)
	{
		return;
	}

	bHit = true;
	PlayOnHitEffects(InLocation_WS);
}

void AAuraProjectile::OnLifeEnded()
{
	if (bHit || !HasAuthority())
	{
		return;
	}

	bHit = true;
	PlayOnLifeEndedEffects();

	MulticastOnLifeEnded();

	Destroy();
}

void AAuraProjectile::MulticastOnLifeEnded_Implementation()
{
	if (bHit)
	{
		return;
	}

	bHit = true;
	PlayOnLifeEndedEffects();
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
