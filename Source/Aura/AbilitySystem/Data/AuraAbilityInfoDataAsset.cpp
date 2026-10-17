// Copyright Diegothic


#include "AuraAbilityInfoDataAsset.h"

#include "AbilitySystem/Log/AuraAbilitySystemLog.h"

TOptional<const FAuraAbilityInfo*> UAuraAbilityInfoDataAsset::FindAbilityInfo(
	const FGameplayTag& InAbilityTag,
	const bool bInLogNotFound
) const
{
	const FAuraAbilityInfo* const FoundAbilityInfo = Algo::FindBy(
		AbilityInfos,
		InAbilityTag, &FAuraAbilityInfo::AbilityTag
	);

	if (FoundAbilityInfo != nullptr)
	{
		return FoundAbilityInfo;
	}

	if (bInLogNotFound)
	{
		UE_LOG(
			LogAuraAbilitySystem,
			Error,
			TEXT("[AbilityInfo] Ability info not found for Tag \'%s\'"), *InAbilityTag.ToString()
		);
	}

	return NullOpt;
}
