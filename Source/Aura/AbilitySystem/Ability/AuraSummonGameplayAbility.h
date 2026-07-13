// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraSummonGameplayAbility.generated.h"

UCLASS()
class AURA_API UAuraSummonGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	TArray<FVector> FindSummonLocations() const;

	UFUNCTION(BlueprintPure)
	TSubclassOf<APawn> ChooseSummonMinionClass() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summon")
	int32 NumMinions = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summon")
	float MinSummonDistance_Cm = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summon")
	float MaxSummonDistance_Cm = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summon")
	float SummonSpreadAngle_Deg = 90.0f;
};
