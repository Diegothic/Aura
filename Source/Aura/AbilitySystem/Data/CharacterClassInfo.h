// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "CharacterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger,
};

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

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|CommonClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> SecondaryAttributesGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|CommonClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> VitalAttributesGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|ClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassDefaultInfo;
};
