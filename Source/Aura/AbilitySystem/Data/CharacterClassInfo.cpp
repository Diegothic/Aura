// Copyright Diegothic


#include "CharacterClassInfo.h"


bool Aura::CharacterClass::IsRangedAttacker(const ECharacterClass InCharacterClass)
{
	switch (InCharacterClass)
	{
	case ECharacterClass::Elementalist:
	case ECharacterClass::Ranger:
		return true;
	case ECharacterClass::Warrior:
		return false;
	default:
		return false;
	}
}

const FCharacterClassDefaultInfo& UCharacterClassInfo::GetCharacterClassDefaultInfo(
	const ECharacterClass CharacterClass
) const
{
	return CharacterClassDefaultInfo.FindChecked(CharacterClass);
}

float UCharacterClassInfo::EvaluateDamageCalculationCoefficient(
	const FName& InCoefficientName,
	const float InDefaultValue,
	const int32 InCharacterLevel
) const
{
	if (!IsValid(DamageCalculationCoefficientsTable))
	{
		return InDefaultValue;
	}

	if (const FRealCurve* const CoefficientCurve
			= DamageCalculationCoefficientsTable->FindCurve(InCoefficientName, FString{});
		CoefficientCurve != nullptr
	)
	{
		return CoefficientCurve->Eval(
			InCharacterLevel,
			InDefaultValue
		);
	}

	return InDefaultValue;
}
