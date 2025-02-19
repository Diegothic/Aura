// Copyright Diegothic


#include "AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"


UAuraAttributeSet::UAuraAttributeSet()
	: Health(100.0f),
	  MaxHealth(100.0f),
	  Mana(50.0f),
	  MaxMana(50.0f)
{
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
