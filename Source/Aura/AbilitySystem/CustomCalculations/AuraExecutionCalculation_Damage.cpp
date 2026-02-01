// Copyright Diegothic


#include "AuraExecutionCalculation_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct FAuraDamageStatics
{
public:
	FAuraDamageStatics()
	{
		ArmorProperty = UAuraAttributeSet::GetArmorAttribute().GetUProperty();
		ArmorDef = FGameplayEffectAttributeCaptureDefinition{
			ArmorProperty,
			EGameplayEffectAttributeCaptureSource::Target,
			false
		};
	}

	FProperty* ArmorProperty;
	FGameplayEffectAttributeCaptureDefinition ArmorDef;
};

static const FAuraDamageStatics& DamageStatics()
{
	static FAuraDamageStatics Instance;
	return Instance;
}

UAuraExecutionCalculation_Damage::UAuraExecutionCalculation_Damage()
{
	RelevantAttributesToCapture.Emplace(DamageStatics().ArmorDef);
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

	float ArmorMagnitude = 0.0f;
	if (ExecutionParams.
		AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvalParams, ArmorMagnitude))
	{
		ArmorMagnitude = FMath::Max(ArmorMagnitude, 0.0f);
	}
	ArmorMagnitude += 1.0f;

	const FGameplayModifierEvaluatedData OutEvalData{
		DamageStatics().ArmorProperty,
		EGameplayModOp::AddBase,
		ArmorMagnitude
	};
	OutExecutionOutput.AddOutputModifier(OutEvalData);
}
