// Copyright Diegothic


#include "AuraAbilitySystemStatics.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemTypes.h"
#include "GameplayEffectTypes.h"
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
	const UObject* WorldContextObject,
	UAbilitySystemComponent* DestASC
)
{
	check(DestASC);

	if (const UCharacterClassInfo* const CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
		IsValid(CharacterClassInfo)
	)
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->GetCommonAbilities())
		{
			const FGameplayAbilitySpec AbilitySpec{AbilityClass, 1};
			DestASC->GiveAbility(AbilitySpec);
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
