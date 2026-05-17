// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FAuraGameplayEffectContext();

	FAuraGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser);

	virtual ~FAuraGameplayEffectContext() override;

	bool GetIsBlockedHit() const;

	void SetIsBlockedHit(bool bInIsBlockedHit);

	bool GetIsCriticalHit() const;

	void SetIsCriticalHit(bool bInIsCriticalHit);

	// ~ Begin FGameplayEffectContext
	/// Returns the actual struct used for serialization; subclasses must override this!
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FAuraGameplayEffectContext* Duplicate() const override;

	/// Custom serialization, subclasses must override this
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	// ~ End FGameplayEffectContext

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
