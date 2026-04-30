// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AuraExecutionCalculation_Damage.generated.h"

UCLASS()
class AURA_API UAuraExecutionCalculation_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UAuraExecutionCalculation_Damage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;

private:
	static float FindCapturedAttributeMagnitude(
		const FGameplayEffectCustomExecutionParameters& InExecParams,
		const FAggregatorEvaluateParameters& InEvalParams,
		const FGameplayEffectAttributeCaptureDefinition& InAttributeCaptureDef,
		TOptional<float> InOptMin = NullOpt,
		TOptional<float> InOptMax = NullOpt
	);

	static float ApplyBlockChance(
		float InCurrentDamage,
		const FGameplayEffectCustomExecutionParameters& InExecParams,
		const FAggregatorEvaluateParameters& InEvalParams
	);

	static float ApplyArmor(
		const UObject* InWorldContextObject,
		float InCurrentDamage,
		const FGameplayEffectCustomExecutionParameters& InExecParams,
		const FAggregatorEvaluateParameters& InEvalParams
	);
};
