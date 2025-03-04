// Copyright Diegothic


#include "AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	// Lazy init OverlayWidgetController
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->Initialize(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	// Lazy init AttributeMenuWidgetController
	if (!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->Initialize(Params);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

void AAuraHUD::Initialize(
	APlayerController* InPlayerController,
	APlayerState* InPlayerState,
	UAbilitySystemComponent* InAbilitySystemComponent,
	UAttributeSet* InAttributeSet
)
{
	const FWidgetControllerParams WidgetControllerParams(
		InPlayerController,
		InPlayerState,
		InAbilitySystemComponent,
		InAttributeSet
	);

	checkf(
		OverlayWidgetControllerClass,
		TEXT("[Aura HUD] Overlay Widget Controller Class is not set!")
	);
	UOverlayWidgetController* const OverlayController = GetOverlayWidgetController(WidgetControllerParams);

	checkf(OverlayWidgetClass, TEXT("[Aura HUD] Overlay Widget Class is not set!"));
	UUserWidget* const Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	OverlayWidget->SetWidgetController(OverlayController);

	OverlayController->BroadcastInitialValues();

	Widget->AddToViewport();
}
