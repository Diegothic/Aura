// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraEnemyInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UAuraEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IAuraEnemyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
	virtual AActor* GetCombatTarget_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InTargetActor);
	virtual void SetCombatTarget_Implementation(AActor* InTargetActor);
};
