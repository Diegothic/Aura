// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/Interaction/TargetInterface.h"

#include "AuraEnemyCharacter.generated.h"

UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	AAuraEnemyCharacter();

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	//~ End AActor Interface

public:
	//~ Begin ITargetInterface Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End ITargetInterface Interface

	//~ Begin ICombatInterface Interface
	virtual int32 GetCharacterLevel() const override;
	//~ End ICombatInterface Interface

protected:
	virtual void InitAbilityActorInfo() override;

private:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Aura|Target", Meta = (AllowPrivateAccess = "true"))
	bool bHighlighted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	int32 Level = 1;
};
