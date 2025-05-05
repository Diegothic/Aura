// Copyright Diegothic


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"


FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::Init()
{
	// Primary Attribute Tags
	Instance.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Strength"),
		FString("Increases physical Damage")
	);
	Instance.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"),
		FString("Increases magical Damage")
	);
	Instance.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
	);
	Instance.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Vigor"),
		FString("Increases Health")
	);

	// Secondary Attribute Tags
	Instance.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);
	Instance.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
	);
	Instance.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance")
	);
	Instance.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ArmorPenetration"),
		FString("Ignored Percentage of enemy Armor, increases Critical Hit Chance")
	);
	Instance.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.BlockChance"),
		FString("Chance to cut incoming Damage in half")
	);
	Instance.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitChance"),
		FString("Chance to double Damage plus critical hit bonus")
	);
	Instance.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitDamage"),
		FString("Bonus Damage for critical hits")
	);
	Instance.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
	);
	Instance.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every second")
	);
	Instance.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every second")
	);

	// Input Tags
	Instance.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"),
		FString("Left Mouse Button Input")
	);
	Instance.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"),
		FString("Right Mouse Button Input")
	);
	Instance.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"),
		FString("Key 1 Input")
	);
	Instance.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.2"),
		FString("Key 2 Input")
	);
	Instance.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.3"),
		FString("Key 3 Input")
	);
	Instance.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.4"),
		FString("Key 4 Input")
	);

	// Event Tags
	Instance.Event_Montage_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.FireBolt"),
		FString("Fire Bolt Montage Event")
	);
}
