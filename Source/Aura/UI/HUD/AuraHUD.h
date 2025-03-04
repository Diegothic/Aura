// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "AuraHUD.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAuraUserWidget;
struct FWidgetControllerParams;

UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	void Initialize(
		APlayerController* InPlayerController,
		APlayerState* InPlayerState,
		class UAbilitySystemComponent* InAbilitySystemComponent,
		class UAttributeSet* InAttributeSet
	);

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& Params);

private:
	UPROPERTY(EditAnywhere, Category = "Aura|HUD")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Aura|HUD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "Aura|HUD")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Aura|HUD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "Aura|HUD")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Aura|HUD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
};
