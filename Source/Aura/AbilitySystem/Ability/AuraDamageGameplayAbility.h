// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura|Damage")
	void GatherDamageTypes(FGameplayTagContainer& OutDamageTypeTags) const;

protected:
	TOptional<FGameplayEffectSpecHandle> MakeDamageEffectSpec(
		const FGameplayEffectContextHandle& InEffectContext
	) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageByDamageType;
};
