// Copyright Diegothic

#pragma once

#include "GameplayTagContainer.h"

class FAuraGameplayTags
{
public:
	static AURA_API void Init();

	FORCEINLINE static const FAuraGameplayTags& Get() { return Instance; }

public:
	//~ Begin Primary Attribute Tags
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;
	//~ End Primary Attribute Tags

	//~ Begin Secondary Attribute Tags
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
	//~ End Secondary Attribute Tags

	//~ Begin Input Tags
	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;
	//~ End Input Tags

	//~ Begin Event Tags
	FGameplayTag Event_Montage_FireBolt;
	//~ End Event Tags

private:
	static AURA_API FAuraGameplayTags Instance;
};
