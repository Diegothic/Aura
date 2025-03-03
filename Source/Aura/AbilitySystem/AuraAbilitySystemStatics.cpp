// Copyright Diegothic


#include "AuraAbilitySystemStatics.h"

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
