// Copyright Diegothic


#include "AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"


void UAuraProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();
	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}

	const AActor* const AvatarActor = GetAvatarActorFromActorInfo();
	AActor* const OwningActor = GetOwningActorFromActorInfo();

	FTransform SpawnTransform;
	if (const ICombatInterface* const CombatInterface = Cast<ICombatInterface>(AvatarActor))
	{
		const FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	}

	AAuraProjectile* const SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		OwningActor,
		Cast<APawn>(OwningActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (!IsValid(SpawnedProjectile))
	{
		return;
	}

	if (const UAbilitySystemComponent* const ASC = GetAbilitySystemComponentFromActorInfo(); IsValid(ASC))
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.SetAbility(this);
		EffectContext.AddSourceObject(SpawnedProjectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Emplace(SpawnedProjectile);
		EffectContext.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = TargetLocation;
		EffectContext.AddHitResult(HitResult);

		if (const TOptional<FGameplayEffectSpecHandle> EffectSpecHandleOpt = MakeDamageEffectSpec(EffectContext);
			EffectSpecHandleOpt.IsSet()
		)
		{
			SpawnedProjectile->SetDamageEffectSpecHandle(EffectSpecHandleOpt.GetValue());
		}
	}

	SpawnedProjectile->FinishSpawning(SpawnTransform);
}
