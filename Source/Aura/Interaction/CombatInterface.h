// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FAuraTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> ImpactSound = nullptr;
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	FAuraTaggedMontage GetMatchingAttackMontage(const FGameplayTagContainer& InMontageTags);
	virtual FAuraTaggedMontage GetMatchingAttackMontage_Implementation(const FGameplayTagContainer& InMontageTags);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	UNiagaraSystem* GetHitImpactEffect();
	virtual UNiagaraSystem* GetHitImpactEffect_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	int32 GetMinionCount();
	virtual int32 GetMinionCount_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	int32 ChangeMinionCount(int32 InValueChange);
	virtual int32 ChangeMinionCount_Implementation(int32 InValueChange);
};
