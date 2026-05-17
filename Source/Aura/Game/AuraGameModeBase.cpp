// Copyright Diegothic


#include "AuraGameModeBase.h"

#include "Kismet/GameplayStatics.h"

AAuraGameModeBase* AAuraGameModeBase::Get(const UObject* InWorldContextObject)
{
	if (AAuraGameModeBase* const AuraGM
		= Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject))
	)
	{
		return AuraGM;
	}

	return nullptr;
}

const UCharacterClassInfo* AAuraGameModeBase::GetCharacterClassInfo() const
{
	check(CharacterClassInfo);
	return CharacterClassInfo;
}

const UAuraAbilitySystemDamageConfig* AAuraGameModeBase::GetAbilitySystemDamageConfig() const
{
	check(AbilitySystemDamageConfig);
	return AbilitySystemDamageConfig;
}
