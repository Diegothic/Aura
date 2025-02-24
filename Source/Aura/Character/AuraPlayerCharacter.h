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

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//~ End APawn Interface

	//~ Begin ICombatInterface Interface
	virtual int32 GetCharacterLevel() const override;
	//~ End ICombatInterface Interface

protected:
	virtual void InitAbilityActorInfo() override;

private:
	void InitHUD() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;
};
