// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "AuraPlayerController.generated.h"

class UAuraInputConfig;
class ITargetInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;
struct FInputActionValue;
struct FGameplayTag;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	//~ Begin APlayerController Interface
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void SetupInputComponent() override;
	//~ End APlayerController Interface

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

private:
	void OnMoveTriggered(const FInputActionValue& Value);

	void OnAbilityActionPressed(FGameplayTag InputTag);
	void OnAbilityActionReleased(FGameplayTag InputTag);
	void OnAbilityActionHeld(FGameplayTag InputTag);

	void CursorTrace();

private:
	UAuraAbilitySystemComponent* GetAuraASC();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	TScriptInterface<ITargetInterface> LastTarget;
	TScriptInterface<ITargetInterface> CurrentTarget;
	bool bIsTargeting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Movement", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Aura|Movement")
	float ShortPressThreshold = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Movement")
	float AutoRunAcceptanceRadius = 50.0f;

	FVector CachedDestination = FVector::ZeroVector;
	float CursorTraceTime = 0.0f;
	bool bIsAutoRunning = false;
};
