// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
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

protected:
	UPROPERTY(VisibleAnywhere, Category = "Aura|AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	FName WeaponSocket;
};
