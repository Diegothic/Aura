// Copyright Diegothic


#include "MMC_MaxMana.h"

#include "AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* const SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* const TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	float IntelligenceMagnitude = 0.0f;
	if (GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvalParams, IntelligenceMagnitude))
	{
		IntelligenceMagnitude = FMath::Max(IntelligenceMagnitude, 0.0f);
	}

	int32 CharacterLevel = 0;
	const UObject* const SourceObject = Spec.GetContext().GetSourceObject();
	if (SourceObject->Implements<UCombatInterface>())
	{
		const ICombatInterface* const CombatInterface = Cast<ICombatInterface>(SourceObject);
		CharacterLevel = CombatInterface->GetCharacterLevel();
	}

	return 40.0f + (IntelligenceMagnitude * 1.25f) + (CharacterLevel * 5.0f);
}
