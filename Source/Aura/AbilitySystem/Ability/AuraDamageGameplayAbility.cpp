// Copyright Diegothic


#include "AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::GatherDamageTypes(FGameplayTagContainer& OutDamageTypeTags) const
{
	for (const TTuple<FGameplayTag, FScalableFloat>& DamageType : DamageByDamageType)
	{
		const FGameplayTag& DamageTypeTag = DamageType.Key;
		OutDamageTypeTags.AddTag(DamageTypeTag);
	}
}

TOptional<FGameplayEffectSpecHandle> UAuraDamageGameplayAbility::MakeDamageEffectSpec(
	const FGameplayEffectContextHandle& InEffectContext) const
{
	if (const UAbilitySystemComponent* const ASC = GetAbilitySystemComponentFromActorInfo(); IsValid(ASC))
	{
		const int32 AbilityLevel = GetAbilityLevel();
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(
			DamageGameplayEffect,
			AbilityLevel,
			InEffectContext
		);

		for (const TTuple<FGameplayTag, FScalableFloat>& DamageType : DamageByDamageType)
		{
			const FGameplayTag& DamageTypeTag = DamageType.Key;
			const FScalableFloat& Damage = DamageType.Value;
			const float ScaledDamage = Damage.GetValueAtLevel(AbilityLevel);

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
				EffectSpecHandle,
				DamageTypeTag,
				ScaledDamage
			);
		}

		return EffectSpecHandle;
	}

	return NullOpt;
}
