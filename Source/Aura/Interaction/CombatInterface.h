// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FAuraTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag = FGameplayTag::EmptyTag;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	FVector GetCombatSocketLocation(const FGameplayTag& InMontageTag) const;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& InMontageTag) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	void SetFacingTagetLocation(const FVector& TargetLocation);
	virtual void SetFacingTagetLocation_Implementation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	UAnimMontage* GetHitReactMontage() const;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	bool IsDead() const;
	virtual bool IsDead_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	AActor* GetAvatarActor();
	virtual AActor* GetAvatarActor_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	TArray<FAuraTaggedMontage> GetAttackMontages();
	virtual TArray<FAuraTaggedMontage> GetAttackMontages_Implementation();
};
