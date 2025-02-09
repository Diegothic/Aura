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

public:
	//~ Begin ITargetInterface Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End ITargetInterface Interface

private:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Aura|Target", Meta = (AllowPrivateAccess = "true"))
	bool bHighlighted;
};
