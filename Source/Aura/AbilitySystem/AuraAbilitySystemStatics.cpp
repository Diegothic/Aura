// Copyright Diegothic


#include "AuraAbilitySystemStatics.h"

#include "AbilitySystemComponent.h"
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
	if (const AAuraGameModeBase* const AuraGM = Cast<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)
	))
	{
		const AActor* const AvatarActor = DestASC->GetAvatarActor();

		const UCharacterClassInfo& CharacterClassInfo = AuraGM->GetCharacterClassInfo();
		const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo.GetCharacterClassDefaultInfo(
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
			CharacterClassInfo.GetSecondaryAttributesGameplayEffect(),
			CharacterLevel,
			EffectContext
		);
		DestASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

		const FGameplayEffectSpecHandle VitalAttributesSpecHandle = DestASC->MakeOutgoingSpec(
			CharacterClassInfo.GetVitalAttributesGameplayEffect(),
			CharacterLevel,
			EffectContext
		);
		DestASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	}
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
