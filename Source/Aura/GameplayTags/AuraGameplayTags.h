// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraGameplayTags.generated.h"

UCLASS(MinimalAPI)
class UAuraGameplayTags : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static AURA_API void Init();

	FORCEINLINE static const UAuraGameplayTags& Get() { return *Instance; }

public:
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;

	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;

private:
	static AURA_API UAuraGameplayTags* Instance;
};
