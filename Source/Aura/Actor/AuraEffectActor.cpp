// Copyright Diegothic


#include "AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* const NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(NewRootComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AAuraEffectActor::ApplyEffectToTarget(
	AActor* const InTargetActor,
	const TSubclassOf<UGameplayEffect>& InEffectClass,
	FActiveGameplayEffectHandle& OutEffectHandle
) const
{
	if (UAbilitySystemComponent* const TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		InTargetActor))
	{
		check(InEffectClass);

		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpecHandle
			= TargetASC->MakeOutgoingSpec(InEffectClass, EffectLevel, EffectContextHandle);

		OutEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		return true;
	}

	return false;
}

bool AAuraEffectActor::OnOverlap(AActor* const TargetActor)
{
	bool bAnyEffectApplied = false;

	for (auto& Effect : InstantEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			FActiveGameplayEffectHandle EffectHandle;
			bAnyEffectApplied |= ApplyEffectToTarget(TargetActor, Effect.GameplayEffectClass, EffectHandle);
		}
	}

	for (auto& Effect : DurationEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			FActiveGameplayEffectHandle EffectHandle;
			bAnyEffectApplied |= ApplyEffectToTarget(TargetActor, Effect.GameplayEffectClass, EffectHandle);
		}
	}

	for (auto& Effect : InfiniteEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			FActiveGameplayEffectHandle EffectHandle;
			if (bool bEffectApplied = ApplyEffectToTarget(TargetActor, Effect.GameplayEffectClass, EffectHandle))
			{
				bAnyEffectApplied |= bEffectApplied;
				if (Effect.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
				{
					Effect.ActiveOverlapGameplayEffects.Add(TargetActor, EffectHandle);
				}
			}
		}
	}

	if (bDestroyActorOnEffectApplication && bAnyEffectApplied)
	{
		Destroy();
	}

	return bAnyEffectApplied;
}

bool AAuraEffectActor::OnEndOverlap(AActor* const TargetActor)
{
	bool bAnyEffectApplied = false;
	bool bAnyEffectRemoved = false;

	for (auto& Effect : InstantEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			FActiveGameplayEffectHandle EffectHandle;
			bAnyEffectApplied |= ApplyEffectToTarget(TargetActor, Effect.GameplayEffectClass, EffectHandle);
		}
	}

	for (auto& Effect : DurationEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			FActiveGameplayEffectHandle EffectHandle;
			bAnyEffectApplied |= ApplyEffectToTarget(TargetActor, Effect.GameplayEffectClass, EffectHandle);
		}
	}

	for (auto& Effect : InfiniteEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			FActiveGameplayEffectHandle EffectHandle;
			bAnyEffectApplied |= ApplyEffectToTarget(TargetActor, Effect.GameplayEffectClass, EffectHandle);
		}

		if (Effect.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			if (Effect.ActiveOverlapGameplayEffects.Contains(TargetActor))
			{
				FActiveGameplayEffectHandle EffectHandle;
				Effect.ActiveOverlapGameplayEffects.RemoveAndCopyValue(TargetActor, EffectHandle);

				UAbilitySystemComponent* const TargetASC
					= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
				if (IsValid(TargetASC))
				{
					bAnyEffectRemoved |= TargetASC->RemoveActiveGameplayEffect(EffectHandle, 1);
				}
			}
		}
	}

	if (bDestroyActorOnEffectApplication && bAnyEffectApplied)
	{
		Destroy();
	}

	if (bDestroyActorOnEffectRemoval && bAnyEffectRemoved)
	{
		Destroy();
	}

	return bAnyEffectApplied || bAnyEffectRemoved;
}
