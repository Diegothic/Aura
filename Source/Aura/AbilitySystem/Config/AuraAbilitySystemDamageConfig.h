// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilitySystemDamageConfig.generated.h"

UCLASS()
class AURA_API UAuraAbilitySystemDamageConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TConstArrayView<FGameplayTag> GetDamageTypes() const { return DamageTypes; }

	FORCEINLINE TConstArrayView<FGameplayTag> GetResistanceAttributes() const { return ResistanceAttributes; }

	TOptional<FGameplayTag> GetResistanceByDamageType(const FGameplayTag& DamageType) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TArray<FGameplayTag> DamageTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TArray<FGameplayTag> ResistanceAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TMap<FGameplayTag, FGameplayTag> ResistanceAttributeByDamageType;
};
