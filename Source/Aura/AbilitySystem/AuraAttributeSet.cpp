// Copyright Diegothic


#include "AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameplayTags/AuraGameplayTags.h"


UAuraAttributeSet::UAuraAttributeSet()
	: Health(100.0f),
	  Mana(50.0f),
	  Strength(10.0f),
	  Intelligence(10.0f),
	  Resilience(10.0f),
	  Vigor(10.0f),
	  MaxHealth(100.0f),
	  MaxMana(50.0f)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Primary Attributes
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Primary_Strength,
		GetStrengthAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Primary_Intelligence,
		GetIntelligenceAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Primary_Resilience,
		GetResilienceAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Primary_Vigor,
		GetVigorAttribute
	);

	// Secondary Attributes
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_MaxHealth,
		GetMaxHealthAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_MaxMana,
		GetMaxManaAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_Armor,
		GetArmorAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_ArmorPenetration,
		GetArmorPenetrationAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_BlockChance,
		GetBlockChanceAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_CriticalHitChance,
		GetCriticalHitChanceAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_CriticalHitDamage,
		GetCriticalHitDamageAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_CriticalHitResistance,
		GetCriticalHitResistanceAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_HealthRegeneration,
		GetHealthRegenerationAttribute
	);
	GameplayTagToAttributeGetterMap.Add(
		GameplayTags.Attribute_Secondary_ManaRegeneration,
		GetManaRegenerationAttribute
	);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}

	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProps;
	FillEffectProperties(Data, EffectProps);

	FGameplayAttribute ChangedAttribute = Data.EvaluatedData.Attribute;

	if (ChangedAttribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (ChangedAttribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(GetMaxHealth(), 0.0f));
	}

	if (ChangedAttribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (ChangedAttribute == GetMaxManaAttribute())
	{
		SetMaxMana(FMath::Max(GetMaxMana(), 0.0f));
	}
}

TOptional<TStaticFuncPtr<FGameplayAttribute()>> UAuraAttributeSet::FindGameplayAttributeGetter(
	const FGameplayTag& GameplayTag
) const
{
	if (!GameplayTagToAttributeGetterMap.Contains(GameplayTag))
	{
		return TOptional<TStaticFuncPtr<FGameplayAttribute()>>();
	}

	return GameplayTagToAttributeGetterMap[GameplayTag];
}

void UAuraAttributeSet::FillEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps)
{
	OutProps.EffectContextHandle = Data.EffectSpec.GetContext();
	if ((OutProps.InstigatorASC = OutProps.EffectContextHandle.GetInstigatorAbilitySystemComponent()))
	{
		if (
			OutProps.InstigatorASC->AbilityActorInfo.IsValid()
			&& OutProps.InstigatorASC->AbilityActorInfo->AvatarActor.IsValid()
		)
		{
			OutProps.InstigatorAvatarActor = OutProps.InstigatorASC->GetAvatarActor();

			AController* InstigatorController = OutProps.InstigatorASC->AbilityActorInfo->PlayerController.Get();
			if (!InstigatorController && OutProps.InstigatorAvatarActor)
			{
				if (const APawn* const InstigatorPawn = Cast<APawn>(OutProps.InstigatorAvatarActor))
				{
					InstigatorController = InstigatorPawn->GetController();
				}
			}
			OutProps.InstigatorController = InstigatorController;

			if (InstigatorController)
			{
				OutProps.InstigatorCharacter = Cast<ACharacter>(InstigatorController->GetPawn());
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProps.TargetAvatarActor = Data.Target.GetAvatarActor();
		OutProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		OutProps.TargetCharacter = Cast<ACharacter>(OutProps.TargetAvatarActor);
		OutProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProps.TargetAvatarActor);
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldValue);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldValue);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldValue);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldValue);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldValue);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldValue);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldValue);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldValue);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldValue);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldValue);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldValue);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldValue);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldValue);
}
