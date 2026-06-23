// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura|Damage")
	void GatherDamageTypes(FGameplayTagContainer& OutDamageTypeTags) const;

	UFUNCTION(BlueprintCallable, Category = "Aura|Damage")
	FAuraTaggedMontage GetRandomAttackMontage() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Aura|Damage", DisplayName = "Cause Damage")
	void CauseDamage(AActor* InTargetActor) const;

	TOptional<FGameplayEffectSpecHandle> MakeDamageEffectSpec(
		const FGameplayEffectContextHandle& InEffectContext
	) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageByDamageType;
};
