// Copyright Diegothic


#include "AuraAbilitySystemDamageConfig.h"


TOptional<FGameplayTag> UAuraAbilitySystemDamageConfig::GetResistanceByDamageType(
	const FGameplayTag& DamageType) const
{
	if (!ResistanceAttributeByDamageType.Contains(DamageType))
	{
		return NullOpt;
	}

	return ResistanceAttributeByDamageType[DamageType];
}
