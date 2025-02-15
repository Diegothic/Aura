// Copyright Diegothic


#include "AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGameplayEffectApplied);
}

void UAuraAbilitySystemComponent::OnGameplayEffectApplied(
	UAbilitySystemComponent* InAbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle EffectHandle
) const
{
	FGameplayTagContainer EffectTags;
	EffectSpec.GetAllAssetTags(EffectTags);

	EffectAssetTagsDelegate.Broadcast(EffectTags);
}
