// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	UFUNCTION(BlueprintCallable, Category = "Aura|AppliedEffects")
	void ApplyEffectToTarget(AActor* const InTargetActor, const TSubclassOf<UGameplayEffect>& InEffectClass);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
