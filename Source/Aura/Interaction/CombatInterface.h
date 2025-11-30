// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

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

	virtual FVector GetCombatSocketLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	void SetFacingTagetLocation(const FVector& TargetLocation);
	virtual void SetFacingTagetLocation_Implementation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Aura|Combat")
	UAnimMontage* GetHitReactMontage() const;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const;
	
	virtual void Die();
};
