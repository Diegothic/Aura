// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AuraPickupMovementComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent, DisplayName="Aura Pickup Movement"))
class AURA_API UAuraPickupMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAuraPickupMovementComponent();

	// ~ UActorComponent
	virtual void BeginPlay() override;

	virtual void TickComponent(
		float InDeltaTime,
		ELevelTick InTickType,
		FActorComponentTickFunction* InThisTickFunction
	) override;
	// ~ UActorComponent

private:
	UPROPERTY(EditDefaultsOnly, Category="Aura|Pickup")
	float MaxFloatingOffset = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Pickup")
	float FloatingSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Pickup")
	float RotationSpeed = 360.0f;

	FVector InitialActorLocation;

	FRotator InitialActorRotation;
};
