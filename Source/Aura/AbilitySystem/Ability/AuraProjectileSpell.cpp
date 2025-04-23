// Copyright Diegothic


#include "AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthority(&ActivationInfo))
	{
		AActor* const OwningActor = GetOwningActorFromActorInfo();
		const AActor* const AvatarActor = GetAvatarActorFromActorInfo();

		FTransform SpawnTransform;
		if (const ICombatInterface* const CombatInterface = Cast<ICombatInterface>(AvatarActor))
		{
			const FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
			// TODO: Get rotation to the Target
			const FRotator SpawnRotation = AvatarActor->GetActorRotation();
			SpawnTransform.SetLocation(SpawnLocation);
			SpawnTransform.SetRotation(FQuat(SpawnRotation));
		}

		AAuraProjectile* const SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			OwningActor,
			Cast<APawn>(OwningActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		// TODO: Give the projectile a Gameplay Effect Spec for causing Damage

		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
