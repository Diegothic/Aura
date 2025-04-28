// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	FORCEINLINE const FGameplayTag& GetStartupInputTag() const {return StartupInputTag;}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Input")
	FGameplayTag StartupInputTag;
};
