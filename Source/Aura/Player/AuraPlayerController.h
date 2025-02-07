// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void OnMoveTriggered(const struct FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveInputAction;
};
