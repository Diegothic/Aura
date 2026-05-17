// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "AuraGameModeBase.generated.h"

class UAuraAbilitySystemDamageConfig;
class UCharacterClassInfo;

UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura|GameMode", meta = (WorldContext = "InWorldContextObject"))
	static AAuraGameModeBase* Get(const UObject* InWorldContextObject);

	UFUNCTION(BlueprintPure)
	const UCharacterClassInfo* GetCharacterClassInfo() const;

	UFUNCTION(BlueprintPure)
	const UAuraAbilitySystemDamageConfig* GetAbilitySystemDamageConfig() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura|CharacterClassDefaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TObjectPtr<UAuraAbilitySystemDamageConfig> AbilitySystemDamageConfig;
};
