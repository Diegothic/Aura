// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger,
};

namespace Aura::CharacterClass
{
	bool IsRangedAttacker(ECharacterClass InCharacterClass);
}

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Aura|ClassDefaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributesGameplayEffect;
};

UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	const FCharacterClassDefaultInfo& GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) const;

	const TSubclassOf<UGameplayEffect>& GetSecondaryAttributesGameplayEffect() const
	{
		return SecondaryAttributesGameplayEffect;
	}

	const TSubclassOf<UGameplayEffect>& GetVitalAttributesGameplayEffect() const
	{
		return VitalAttributesGameplayEffect;
	}

	TConstArrayView<TSubclassOf<UGameplayAbility>> GetCommonAbilities() const
	{
		return CommonAbilities;
	}

	const UCurveTable* GetDamageCalculationCoefficientsTable() const
	{
		return DamageCalculationCoefficientsTable;
	}

	float EvaluateDamageCalculationCoefficient(const FName& InCoefficientName, float InDefaultValue, int32 InCharacterLevel) const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|CommonClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> SecondaryAttributesGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|CommonClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> VitalAttributesGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category="Aura|CommonClassDefaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|CommonClassDefaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficientsTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|ClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassDefaultInfo;
};
