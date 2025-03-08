// Copyright Diegothic


#include "AuraInputConfig.h"

TOptional<const UInputAction*> UAuraInputConfig::FindAbilityInputAction(
	const FGameplayTag& InputTag,
	bool bLogNotFound
) const
{
	for (const auto& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("[AuraInputConfig] Ability Input Action not found for Tag \'%s\'"), *InputTag.ToString()
		);
	}

	return TOptional<const UInputAction*>();
}
