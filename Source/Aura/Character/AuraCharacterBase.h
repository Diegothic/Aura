// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"

#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	//~ Begin AActor Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	//~ End AActor Interface

	//~ Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

public:
	FORCEINLINE USkeletalMeshComponent* GetWeapon() const { return Weapon; }

	//~ Begin ICombatInterface Interface
	virtual FVector GetCombatSocketLocation() const override;
	//~ End ICombatInterface Interface

protected:
	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float Level) const;

	virtual void InitDefaultAttributes() const;

	void GiveStartupAbilities() const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Aura|AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Attributes", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Attributes", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Attributes", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	FName WeaponSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	FName WeaponSpellSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Abilities", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
