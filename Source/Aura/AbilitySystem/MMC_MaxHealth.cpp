// Copyright Diegothic


#include "MMC_MaxHealth.h"

#include "AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* const SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* const TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	float VigorMagnitude = 0.0f;
	if (GetCapturedAttributeMagnitude(VigorDef, Spec, EvalParams, VigorMagnitude))
	{
		VigorMagnitude = FMath::Max(VigorMagnitude, 0.0f);
	}

	int32 CharacterLevel = 0;
	const UObject* const SourceObject = Spec.GetContext().GetSourceObject();
	if (SourceObject && SourceObject->Implements<UCombatInterface>())
	{
		const ICombatInterface* const CombatInterface = Cast<ICombatInterface>(SourceObject);
		CharacterLevel = CombatInterface->GetCharacterLevel();
	}

	return 80.0f + (VigorMagnitude * 2.5f) + (CharacterLevel * 10.0f);
}
