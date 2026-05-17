// Copyright Diegothic


#include "AuraAbilitySystemGlobals.h"

#include "AuraAbilitySystemTypes.h"


FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext{};
}
