// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInfoDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};

UCLASS()
class AURA_API UAuraAbilityInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	TOptional<const FAuraAbilityInfo*> FindAbilityInfo(
		const FGameplayTag& InAbilityTag,
		bool bInLogNotFound = false
	) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraAbilityInfo> AbilityInfos;
};
