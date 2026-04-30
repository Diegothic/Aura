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

		ArmorPenetrationProperty = UAuraAttributeSet::GetArmorPenetrationAttribute().GetUProperty();
		ArmorPenetrationDef = FGameplayEffectAttributeCaptureDefinition{
			ArmorPenetrationProperty,
			EGameplayEffectAttributeCaptureSource::Source,
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

	FProperty* ArmorPenetrationProperty;
	FGameplayEffectAttributeCaptureDefinition ArmorPenetrationDef;

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
	RelevantAttributesToCapture.Emplace(DamageStatics().ArmorPenetrationDef);
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

	FinalDamage = ApplyBlockChance(FinalDamage, ExecutionParams, EvalParams);
	FinalDamage = ApplyArmor(FinalDamage, ExecutionParams, EvalParams);

	const FGameplayModifierEvaluatedData OutEvalData{
		UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		FinalDamage
	};
	OutExecutionOutput.AddOutputModifier(OutEvalData);
}

float UAuraExecutionCalculation_Damage::FindCapturedAttributeMagnitude(
	const FGameplayEffectCustomExecutionParameters& InExecParams,
	const FAggregatorEvaluateParameters& InEvalParams,
	const FGameplayEffectAttributeCaptureDefinition& InAttributeCaptureDef,
	TOptional<float> InOptMin,
	TOptional<float> InOptMax
)
{
	float AttributeMagnitude = InOptMin.IsSet() ? InOptMin.GetValue() : 0.0f;
	bool bCalculated = InExecParams.AttemptCalculateCapturedAttributeMagnitude(
		InAttributeCaptureDef,
		InEvalParams,
		AttributeMagnitude
	);
	if (!bCalculated)
	{
		return AttributeMagnitude;
	}

	if (InOptMin.IsSet())
	{
		AttributeMagnitude = FMath::Max(AttributeMagnitude, InOptMin.GetValue());
	}

	if (InOptMax.IsSet())
	{
		AttributeMagnitude = FMath::Min(AttributeMagnitude, InOptMax.GetValue());
	}

	return AttributeMagnitude;
}

float UAuraExecutionCalculation_Damage::ApplyBlockChance(
	const float InCurrentDamage,
	const FGameplayEffectCustomExecutionParameters& InExecParams,
	const FAggregatorEvaluateParameters& InEvalParams)
{
	const float BlockChanceMagnitude = FindCapturedAttributeMagnitude(
		InExecParams,
		InEvalParams,
		DamageStatics().BlockChanceDef,
		0.0f,
		100.0f
	);

	const bool bBlocked = BlockChanceMagnitude != 0.0f && FMath::RandRange(0.0f, 100.0f) <= BlockChanceMagnitude;
	if (bBlocked)
	{
		return InCurrentDamage * 0.5f;
	}

	return InCurrentDamage;
}

float UAuraExecutionCalculation_Damage::ApplyArmor(
	const float InCurrentDamage,
	const FGameplayEffectCustomExecutionParameters& InExecParams,
	const FAggregatorEvaluateParameters& InEvalParams
)
{
	const float TargetArmorMagnitude = FindCapturedAttributeMagnitude(
		InExecParams,
		InEvalParams,
		DamageStatics().ArmorDef,
		0.0f
	);

	const float SourceArmorPenetrationMagnitude = FindCapturedAttributeMagnitude(
		InExecParams,
		InEvalParams,
		DamageStatics().ArmorPenetrationDef,
		0.0f
	);

	const float EffectiveArmor = FMath::Max(
		TargetArmorMagnitude * (100.0f - SourceArmorPenetrationMagnitude) / 100.0f,
		0.0f
	);

	return InCurrentDamage * FMath::Max((100.0f - EffectiveArmor) / 100.0f, 1.0f);
}
