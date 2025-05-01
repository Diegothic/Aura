// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "AuraPlayerController.generated.h"

class UAuraInputConfig;
class UInputAction;
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

	void OnCombatActionStarted();
	void OnCombatActionCompleted();

	void OnAbilityActionPressed(FGameplayTag InputTag);
	void OnAbilityActionReleased(FGameplayTag InputTag);
	void OnAbilityActionHeld(FGameplayTag InputTag);

	void CursorTrace();

	bool AutoRun() const;

private:
	UAuraAbilitySystemComponent* GetAuraASC();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CombatInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	FHitResult CursorHit;

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

	bool bCombatActionActive = false;
};
