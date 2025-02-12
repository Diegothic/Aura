// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"

#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

USTRUCT(BlueprintType)
struct FEffectActorGameplayEffectSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Aura|AppliedEffects")
	EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, Category = "Aura|AppliedEffects")
	EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	TMap<TObjectPtr<AActor>, FActiveGameplayEffectHandle> ActiveOverlapGameplayEffects;
};

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
	bool ApplyEffectToTarget(
		AActor* const InTargetActor,
		const TSubclassOf<UGameplayEffect>& InEffectClass,
		FActiveGameplayEffectHandle& OutEffectHandle
	) const;

	/**
	 * Applies the appropriate Gameplay Effects if their Application Policy was set to Apply On Overlap.
	 * @param TargetActor				Actor that will receive the effects
	 * @return							Whether any effect was applied to TargetActor
	 */
	UFUNCTION(BlueprintCallable, Category = "Aura|AppliedEffects")
	bool OnOverlap(AActor* const TargetActor);

	/**
	 * Applies the appropriate Gameplay Effects if their Application Policy was set to Apply On End Overlap.
	 * Removes the appropriate Gameplay Effects if their Removal Policy was set to Remove On End Overlap.
	 * @param TargetActor				Actor that will receive the effects
	 * @return							Whether any effect was applied to or removed from TargetActor
	 */
	UFUNCTION(BlueprintCallable, Category = "Aura|AppliedEffects")
	bool OnEndOverlap(AActor* const TargetActor);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	bool bDestroyActorOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	bool bDestroyActorOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	TArray<FEffectActorGameplayEffectSpec> InstantEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	TArray<FEffectActorGameplayEffectSpec> DurationEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|AppliedEffects", Meta = (AllowPrivateAccess = "true"))
	TArray<FEffectActorGameplayEffectSpec> InfiniteEffects;
};
