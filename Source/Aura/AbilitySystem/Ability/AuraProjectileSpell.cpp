// Copyright Diegothic


#include "AuraProjectileSpell.h"

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
	if (HasAuthority(&ActivationInfo))
	{
		AActor* const OwningActor = GetOwningActorFromActorInfo();
		const AActor* const AvatarActor = GetAvatarActorFromActorInfo();

		FTransform SpawnTransform;
		if (const ICombatInterface* const CombatInterface = Cast<ICombatInterface>(AvatarActor))
		{
			const FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
			FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);
			SpawnRotation.Pitch = 0.0f;

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

		const UAbilitySystemComponent* const ASC = GetAbilitySystemComponentFromActorInfo();
		if (IsValid(ASC))
		{
			const FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(
				DamageGameplayEffect,
				GetAbilityLevel(),
				EffectContext
			);

			SpawnedProjectile->SetDamageEffectSpecHandle(EffectSpecHandle);
		}


		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
