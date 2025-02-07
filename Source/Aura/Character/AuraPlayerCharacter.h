// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraPlayerCharacter.generated.h"

UCLASS()
class AURA_API AAuraPlayerCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraPlayerCharacter();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;
};
