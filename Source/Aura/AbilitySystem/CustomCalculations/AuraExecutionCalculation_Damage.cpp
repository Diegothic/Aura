// Copyright Diegothic


#include "AuraExecutionCalculation_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayTags/AuraGameplayTags.h"

struct FAuraDamageStatics
{
	FAuraDamageStatics()
	{
		ArmorProperty = UAuraAttributeSet::GetArmorAttribute().GetUProperty();
		ArmorDef = FGameplayEffectAttributeCaptureDefinition{
			ArmorProperty,
			EGameplayEffectAttributeCaptureSource::Target,
			false
		};

		BlockChanceProperty = UAuraAttributeSet::GetBlockChanceAttribute().GetUProperty();
		BlockChanceDef = FGameplayEffectAttributeCaptureDefinition{
			BlockChanceProperty,
			EGameplayEffectAttributeCaptureSource::Target,
			false
		};
	}

	FProperty* ArmorProperty;
	FGameplayEffectAttributeCaptureDefinition ArmorDef;

	FProperty* BlockChanceProperty;
	FGameplayEffectAttributeCaptureDefinition BlockChanceDef;
};

static const FAuraDamageStatics& DamageStatics()
{
	static FAuraDamageStatics Instance;
	return Instance;
}

UAuraExecutionCalculation_Damage::UAuraExecutionCalculation_Damage()
{
	RelevantAttributesToCapture.Emplace(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Emplace(DamageStatics().BlockChanceDef);
}

void UAuraExecutionCalculation_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
	const UAbilitySystemComponent* const SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* const TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* const SourceAvatar = IsValid(SourceASC) ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* const TargetAvatar = IsValid(TargetASC) ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* const SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* const TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	float FinalDamage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().GameplayEffect_Damage);

	float BlockChanceMagnitude = 0.0f;
	if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvalParams,
	                                                               BlockChanceMagnitude))
	{
		BlockChanceMagnitude = FMath::Clamp(BlockChanceMagnitude, 0.0f, 100.0f);
	}

	const bool bBlocked = BlockChanceMagnitude != 0.0f && FMath::RandRange(0.0f, 100.0f) <= BlockChanceMagnitude;
	if (bBlocked)
	{
		FinalDamage *= 0.5f;
	}

	const FGameplayModifierEvaluatedData OutEvalData{
		UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		FinalDamage
	};
	OutExecutionOutput.AddOutputModifier(OutEvalData);
}
