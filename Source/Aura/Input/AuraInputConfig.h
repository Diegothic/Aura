// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct AURA_API FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	TOptional<const UInputAction*> FindAbilityInputAction(const FGameplayTag& InputTag, bool bLogNotFound) const;

	FORCEINLINE TConstArrayView<const FAuraInputAction> GetAbilityInputActions() const
	{
		return TConstArrayView<const FAuraInputAction>(AbilityInputActions);
	}

private:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Aura|Input", meta = (AllowPrivateAccess = "true"))
	TArray<FAuraInputAction> AbilityInputActions;
};
