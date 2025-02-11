// Copyright Diegothic


#include "AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	// Lazy init OverlayWidgetController
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->Initialize(Params);
	}

	return OverlayWidgetController;
}

void AAuraHUD::Initialize(
	APlayerController* InPlayerController,
	APlayerState* InPlayerState,
	UAbilitySystemComponent* InAbilitySystemComponent,
	UAttributeSet* InAttributeSet
)
{
	checkf(OverlayWidgetClass, TEXT("[Aura HUD] Overlay Widget Class is not set!"));
	checkf(OverlayWidgetControllerClass, TEXT("[Aura HUD] Overlay Widget Controller Class is not set!"));

	UUserWidget* const Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(
		InPlayerController,
		InPlayerState,
		InAbilitySystemComponent,
		InAttributeSet
	);
	OverlayWidget->SetWidgetController(GetOverlayWidgetController(WidgetControllerParams));

	OverlayWidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
