// Copyright Diegothic


#include "AuraAbilitySystemStatics.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemTypes.h"
#include "GameplayEffectTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"


UOverlayWidgetController* UAuraAbilitySystemStatics::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	if (CreateWidgetControllerParams(WorldContextObject, WidgetControllerParams))
	{
		if (AAuraHUD* const AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>())
		{
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemStatics::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject
)
{
	FWidgetControllerParams WidgetControllerParams;
	if (CreateWidgetControllerParams(WorldContextObject, WidgetControllerParams))
	{
		if (AAuraHUD* const AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>())
		{
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UAuraAbilitySystemStatics::InitDefaultAttributesForClass(
	const UObject* WorldContextObject,
	const ECharacterClass& CharacterClass,
	float CharacterLevel,
	UAbilitySystemComponent* DestASC
)
{
	if (const UCharacterClassInfo* const CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
		IsValid(CharacterClassInfo)
	)
	{
		const AActor* const AvatarActor = DestASC->GetAvatarActor();

		const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(
			CharacterClass
		);

		FGameplayEffectContextHandle EffectContext = DestASC->MakeEffectContext();
		EffectContext.AddSourceObject(AvatarActor);

		const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = DestASC->MakeOutgoingSpec(
			ClassDefaultInfo.PrimaryAttributesGameplayEffect,
			CharacterLevel,
			EffectContext
		);
		DestASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

		const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = DestASC->MakeOutgoingSpec(
			CharacterClassInfo->GetSecondaryAttributesGameplayEffect(),
			CharacterLevel,
			EffectContext
		);
		DestASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

		const FGameplayEffectSpecHandle VitalAttributesSpecHandle = DestASC->MakeOutgoingSpec(
			CharacterClassInfo->GetVitalAttributesGameplayEffect(),
			CharacterLevel,
			EffectContext
		);
		DestASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	}
}

void UAuraAbilitySystemStatics::GiveStartupAbilities(
	const UObject* InWorldContextObject,
	UAbilitySystemComponent* InDestASC,
	const ECharacterClass InCharacterClass
)
{
	check(InDestASC);

	if (const UCharacterClassInfo* const CharacterClassInfo = GetCharacterClassInfo(InWorldContextObject);
		IsValid(CharacterClassInfo)
	)
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->GetCommonAbilities())
		{
			const FGameplayAbilitySpec AbilitySpec{AbilityClass, 1};
			InDestASC->GiveAbility(AbilitySpec);
		}

		const int32 CharacterLevel = [InDestASC]() -> int32
		{
			if (const ICombatInterface* const CombatInterface
					= Cast<ICombatInterface>(InDestASC->GetAvatarActor());
				CombatInterface != nullptr
			)
			{
				return CombatInterface->GetCharacterLevel();
			}

			return 1;
		}();

		const FCharacterClassDefaultInfo& ClassDefaultInfo
			= CharacterClassInfo->GetCharacterClassDefaultInfo(InCharacterClass);
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : ClassDefaultInfo.StartupAbilities)
		{
			const FGameplayAbilitySpec AbilitySpec{AbilityClass, CharacterLevel};
			InDestASC->GiveAbility(AbilitySpec);
		}
	}
}

const UCharacterClassInfo* UAuraAbilitySystemStatics::GetCharacterClassInfo(const UObject* InWorldContextObject)
{
	if (const AAuraGameModeBase* const AuraGM = AAuraGameModeBase::Get(InWorldContextObject); IsValid(AuraGM))
	{
		return AuraGM->GetCharacterClassInfo();
	}

	return nullptr;
}

const UAuraAbilitySystemDamageConfig* UAuraAbilitySystemStatics::GetDamageConfig(const UObject* InWorldContextObject)
{
	if (const AAuraGameModeBase* const AuraGM = AAuraGameModeBase::Get(InWorldContextObject); IsValid(AuraGM))
	{
		return AuraGM->GetAbilitySystemDamageConfig();
	}

	return nullptr;
}

bool UAuraAbilitySystemStatics::GetIsBlockedHit(const FGameplayEffectContextHandle& InEffectContextHandle)
{
	const FAuraGameplayEffectContext* const AuraEffectContext
		= static_cast<const FAuraGameplayEffectContext*>(InEffectContextHandle.Get());
	if (AuraEffectContext == nullptr)
	{
		return false;
	}

	return AuraEffectContext->GetIsBlockedHit();
}

void UAuraAbilitySystemStatics::SetIsBlockedHit(
	FGameplayEffectContextHandle& InEffectContextHandle,
	const bool bInIsBlockedHit
)
{
	FAuraGameplayEffectContext* const AuraEffectContext
		= static_cast<FAuraGameplayEffectContext*>(InEffectContextHandle.Get());
	if (AuraEffectContext == nullptr)
	{
		return;
	}

	AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
}

bool UAuraAbilitySystemStatics::GetIsCriticalHit(const FGameplayEffectContextHandle& InEffectContextHandle)
{
	const FAuraGameplayEffectContext* const AuraEffectContext
		= static_cast<const FAuraGameplayEffectContext*>(InEffectContextHandle.Get());
	if (AuraEffectContext == nullptr)
	{
		return false;
	}

	return AuraEffectContext->GetIsCriticalHit();
}

void UAuraAbilitySystemStatics::SetIsCriticalHit(
	FGameplayEffectContextHandle& InEffectContextHandle,
	const bool bInIsCriticalHit
)
{
	FAuraGameplayEffectContext* const AuraEffectContext
		= static_cast<FAuraGameplayEffectContext*>(InEffectContextHandle.Get());
	if (AuraEffectContext == nullptr)
	{
		return;
	}

	AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
}

void UAuraAbilitySystemStatics::GetAliveCombatActorsInRadius(
	const UObject* InWorldContextObject,
	const FVector& OriginLocation_WS,
	const float Radius_Cm,
	TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore
)
{
	OutOverlappingActors.Empty();

	if (!IsValid(InWorldContextObject))
	{
		return;
	}

	FCollisionQueryParams QueryParams{NAME_None, false};
	QueryParams.MobilityType = EQueryMobilityType::Dynamic;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* const World = InWorldContextObject->GetWorld())
	{
		const FCollisionObjectQueryParams ObjectQueryParams{FCollisionObjectQueryParams::InitType::AllDynamicObjects};
		const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius_Cm);
		World->OverlapMultiByObjectType(
			Overlaps,
			OriginLocation_WS,
			FQuat::Identity,
			ObjectQueryParams,
			CollisionShape,
			QueryParams
		);
	}

	if (Overlaps.IsEmpty())
	{
		return;
	}

	for (const FOverlapResult& Overlap : Overlaps)
	{
		if (AActor* const OverlapActor = Overlap.GetActor();
			IsValid(OverlapActor)
			&& OverlapActor->Implements<UCombatInterface>()
			&& !ICombatInterface::Execute_IsDead(OverlapActor)
		)
		{
			AActor* const CombatAvatar = ICombatInterface::Execute_GetAvatarActor(OverlapActor);
			OutOverlappingActors.AddUnique(CombatAvatar);
		}
	}
}

bool UAuraAbilitySystemStatics::AreActorsFriendly(const AActor* InActorA, const AActor* InActorB)
{
	if (!IsValid(InActorA) || !IsValid(InActorB))
	{
		return false;
	}

	const FName PlayerTag = FName{"Player"};
	const FName EnemyTag = FName{"Enemy"};
	return (InActorA->ActorHasTag(PlayerTag) && InActorB->ActorHasTag(PlayerTag))
		|| (InActorA->ActorHasTag(EnemyTag) && InActorB->ActorHasTag(EnemyTag));
}

bool UAuraAbilitySystemStatics::CreateWidgetControllerParams(
	const UObject* WorldContextObject,
	FWidgetControllerParams& OutParams
)
{
	if (!WorldContextObject)
	{
		return false;
	}

	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		return false;
	}

	AAuraPlayerState* const AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	if (!AuraPlayerState)
	{
		return false;
	}

	UAbilitySystemComponent* const ASC = AuraPlayerState->GetAbilitySystemComponent();
	UAttributeSet* const AttributeSet = AuraPlayerState->GetAttributeSet();
	if (!ASC || !AttributeSet)
	{
		return false;
	}

	OutParams.PlayerController = PlayerController;
	OutParams.PlayerState = AuraPlayerState;
	OutParams.AbilitySystemComponent = ASC;
	OutParams.AttributeSet = AttributeSet;

	return true;
}
