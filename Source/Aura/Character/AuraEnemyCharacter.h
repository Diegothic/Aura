// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/Interaction/TargetInterface.h"

#include "AuraEnemyCharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyAttributeChangedSignature, float, NewValue);

UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	AAuraEnemyCharacter();

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* InNewController) override;
	//~ End APawn Interface

	UPROPERTY(BlueprintAssignable, Category = "Aura|AbilitySystem|Attributes")
	FOnEnemyAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Aura|AbilitySystem|Attributes")
	FOnEnemyAttributeChangedSignature OnMaxHealthChanged;

public:
	//~ Begin ITargetInterface Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End ITargetInterface Interface

	//~ Begin ICombatInterface Interface
	virtual int32 GetCharacterLevel() const override;
	virtual void Die() override;
	//~ End ICombatInterface Interface

	UFUNCTION(BlueprintPure, Category = "Aura|Combat")
	bool IsReactingToHit() const { return bReactingToHit; }

protected:
	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;
	//~ End AAuraCharacterBase Interface

	void BindToAbilitySystemEvents();
	void BroadcastInitialAttributeValues() const;

	void OnHitReactTagChangedEvent(const FGameplayTag InChangedTag, int32 InNewTagCount);

private:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Aura|Target", meta = (AllowPrivateAccess = "true"))
	bool bHighlighted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat", meta = (AllowPrivateAccess = "true"))
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat", meta = (AllowPrivateAccess = "true"))
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "Aura|Combat", meta = (ForceUnits = "s"))
	float DeathLifeSpan = 5.0f;

	bool bReactingToHit = false;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
