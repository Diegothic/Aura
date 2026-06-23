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

FAuraTaggedMontage UAuraDamageGameplayAbility::GetRandomAttackMontage() const
{
	AActor* const AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor))
	{
		return {};
	}

	const TArray<FAuraTaggedMontage> AttackTaggedMontages = ICombatInterface::Execute_GetAttackMontages(AvatarActor);
	if (AttackTaggedMontages.IsEmpty())
	{
		return {};
	}

	const int32 RandIdx = FMath::RandRange(0, AttackTaggedMontages.Num() - 1);
	return AttackTaggedMontages[RandIdx];
}

void UAuraDamageGameplayAbility::CauseDamage(AActor* InTargetActor) const
{
	const UAbilitySystemComponent* const ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC))
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.SetAbility(this);

	if (const TOptional<FGameplayEffectSpecHandle> EffectSpecHandleOpt = MakeDamageEffectSpec(EffectContext);
		EffectSpecHandleOpt.IsSet()
	)
	{
		const FGameplayEffectSpecHandle EffectSpecHandle = EffectSpecHandleOpt.GetValue();
		if (UAbilitySystemComponent* const TargetASC
			= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor)
		)
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		}
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
