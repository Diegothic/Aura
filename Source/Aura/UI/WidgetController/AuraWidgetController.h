// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams(
		APlayerController* InPlayerController,
		APlayerState* InPlayerState,
		UAbilitySystemComponent* InAbilitySystemComponent,
		UAttributeSet* InAttributeSet
	)
		: PlayerController(InPlayerController),
		  PlayerState(InPlayerState),
		  AbilitySystemComponent(InAbilitySystemComponent),
		  AttributeSet(InAttributeSet)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Aura|WidgetController")
	void Initialize(const FWidgetControllerParams& Params);

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
